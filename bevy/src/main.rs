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

#[derive(Resource)]
struct FrameBenchmark {
    start: Option<Instant>,
    durations: Vec<Duration>,
    count: usize,
    file: Option<Mutex<std::fs::File>>,
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
            let mut median_micros: u128 = 0;
            if config.repetitions % 2 == 0 {
                median_micros = (bench.durations.iter().nth(config.repetitions / 2).unwrap().as_nanos() + bench.durations.iter().nth(config.repetitions / 2 - 1).unwrap().as_nanos()) / 2;
            }
            else {
                median_micros = bench.durations.iter().nth(config.repetitions / 2).unwrap().as_nanos();
            }

            let summary = format!(
                "Benchmark over {} entities: avg {:.3}ns, median {:.3}ns, stddev {:.3}ns, min {}ns, max {}ns",
                config.max_entities, avg_micros, median_micros, stddev, min_micros, max_micros
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
    App::new()
        .add_plugins(MinimalPlugins)
        .insert_resource(resources::SimulationConfig {
            max_entities: 8000,
            repetitions: 1000,
            strange_ratio: 0.0,
            entity_rate: 2048,
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
        })
        .add_systems(Startup, setup::test4_setup)
        .add_systems(Update,(
            benchmark_start, 
            (
                systems::counting_system,
            ), 
            benchmark_end
        ).chain())
        .run();
}
