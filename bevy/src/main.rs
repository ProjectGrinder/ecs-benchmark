mod components;
mod resources;
mod setup;
mod systems;

use bevy::app::AppExit;
use bevy::prelude::*;
use std::fs::OpenOptions;
use std::io::Write;
use std::sync::Mutex;
use std::time::Duration;
use std::time::Instant;
use rand::SeedableRng;

#[cfg(windows)]
#[repr(C)]
struct ProcessMemoryCounters {
    cb: u32,
    page_fault_count: u32,
    peak_working_set_size: usize,
    working_set_size: usize,
    quota_peak_paged_pool_usage: usize,
    quota_paged_pool_usage: usize,
    quota_peak_non_paged_pool_usage: usize,
    quota_non_paged_pool_usage: usize,
    pagefile_usage: usize,
    peak_pagefile_usage: usize,
}

#[cfg(windows)]
#[link(name = "psapi")]
unsafe extern "system" {
    fn GetProcessMemoryInfo(
        process: *mut std::ffi::c_void,
        counters: *mut ProcessMemoryCounters,
        size: u32,
    ) -> i32;
}

#[cfg(windows)]
#[link(name = "kernel32")]
unsafe extern "system" {
    fn GetCurrentProcess() -> *mut std::ffi::c_void;
}

fn process_memory_bytes() -> Option<u64> {
    #[cfg(windows)]
    {
        let mut counters = ProcessMemoryCounters {
            cb: std::mem::size_of::<ProcessMemoryCounters>() as u32,
            page_fault_count: 0,
            peak_working_set_size: 0,
            working_set_size: 0,
            quota_peak_paged_pool_usage: 0,
            quota_paged_pool_usage: 0,
            quota_peak_non_paged_pool_usage: 0,
            quota_non_paged_pool_usage: 0,
            pagefile_usage: 0,
            peak_pagefile_usage: 0,
        };

        // SAFETY: the OS functions receive a valid current-process handle and
        // a pointer to a correctly sized, writable counter structure.
        let success = unsafe {
            GetProcessMemoryInfo(
                GetCurrentProcess(),
                &mut counters,
                counters.cb,
            )
        };
        return (success != 0).then_some(counters.working_set_size as u64);
    }

    #[cfg(target_os = "linux")]
    {
        let statm = std::fs::read_to_string("/proc/self/statm").ok()?;
        let resident_pages = statm.split_whitespace().nth(1)?.parse::<u64>().ok()?;
        return Some(resident_pages * 4096);
    }

    #[allow(unreachable_code)]
    None
}

#[derive(Resource)]
struct FrameBenchmark {
    start: Option<Instant>,
    durations: Vec<Duration>,
    count: usize,
    file: Option<Mutex<std::fs::File>>,
    initial_memory: Option<u64>,
}

fn benchmark_start(mut bench: ResMut<FrameBenchmark>) {
    bench.start = Some(Instant::now());
}

fn benchmark_end(
    mut bench: ResMut<FrameBenchmark>,
    config: Res<resources::SimulationConfig>,
    mut exit: MessageWriter<AppExit>,
) {
    if let Some(start) = bench.start.take() {
        let duration = start.elapsed();

        if bench.count >= 2 * config.repetitions {
            return;
        }

        // accumulate
        bench.count += 1;

        // if in front half, no record
        if bench.count <= config.repetitions {
            return;
        }

        bench.durations.push(duration);

        // only log after desired repetitions
        if bench.count >= 2 * config.repetitions {
            let mut total_micros = Duration::new(0,0).as_nanos() as f64;
            for item in bench.durations.iter() {
                total_micros += item.as_nanos() as f64;
            }
            
            let avg_micros = total_micros / (config.repetitions as f64);
            let mut variance: f64 = 0.0;
            for item in bench.durations.iter() {
                variance += ((item.as_nanos() as f64) - avg_micros) * ((item.as_nanos() as f64) - avg_micros);
            }
            variance /= config.repetitions as f64;
            let stddev = f64::sqrt(variance);

            // sort bench durations to find min/max/median
            bench.durations.sort_unstable();

            let min_micros = bench.durations.iter().min().unwrap().as_nanos();
            let max_micros = bench.durations.iter().max().unwrap().as_nanos();
            let median_micros: u128;
            if config.repetitions % 2 == 0 {
                median_micros = (bench.durations.iter().nth(config.repetitions / 2).unwrap().as_nanos() + bench.durations.iter().nth(config.repetitions / 2 - 1).unwrap().as_nanos()) / 2;
            }
            else {
                median_micros = bench.durations.iter().nth(config.repetitions / 2).unwrap().as_nanos();
            }

            let memory_used = process_memory_bytes()
                .zip(bench.initial_memory)
                .map(|(current, initial)| current.saturating_sub(initial));
            let memory_bytes = memory_used
                .map_or_else(|| "unavailable".to_string(), |bytes| bytes.to_string());
            let memory_mib = memory_used
                .map(|bytes| bytes as f64 / (1024.0 * 1024.0))
                .unwrap_or(0.0);

            let summary = format!(
                "Benchmark over {} entities: avg {:.3}ns, median {:.3}ns, stddev {:.3}ns, min {}ns, max {}ns, memory {} bytes ({:.3} MiB)",
                config.max_entities,
                avg_micros,
                median_micros,
                stddev,
                min_micros,
                max_micros,
                memory_bytes,
                memory_mib,
            );

            println!("{}", summary);
            if let Some(mutex) = &bench.file {
                if let Ok(mut file) = mutex.lock() {
                    if let Err(e) = writeln!(file, "{}", summary) {
                        eprintln!("Failed writing benchmark: {}", e);
                    }
                }
            }

            // terminate app
            exit.write(AppExit::Success);
        }
    }
}

// setup

fn main() {
    let initial_memory = process_memory_bytes();

    App::new()
        .add_plugins(MinimalPlugins)
        .insert_resource(resources::RandomNumberGenerator {
            rng: rand::rngs::Xoshiro256PlusPlus::seed_from_u64(776769420),
        })
        .insert_resource(FrameBenchmark{
            start: None,
            durations: Vec::new(),
            count: 0,
            file: OpenOptions::new()
                .create(true)
                .append(true)
                .open("frame_benchmark.log")
                .ok()
                .map(|f| Mutex::new(f)),
            initial_memory,
        })
        .insert_resource(resources::SimulationConfig {
            max_entities: 64000,
            repetitions: 1000,
            strange_ratio: 0.1,
            entity_rate: 2048,
        })
        .add_systems(Startup, setup::test1_setup)
        .add_systems(Update,(
            benchmark_start, 
            (
                systems::movement_system,
            ), 
            benchmark_end
        ).chain())
        .run();
}
