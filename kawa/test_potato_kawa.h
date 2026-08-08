#pragma once

#include <iostream>
#include "ecs.h"
#include <random>
#include <chrono>
#include <fstream>

namespace Test::Potato {
    // global random engine
    inline std::random_device r;
    inline std::default_random_engine engine(r());
    inline std::uniform_real_distribution roll(0.0f, 1.0f);
    inline std::uniform_int_distribution int_roll(0, 1);

    template <typename T>
    constexpr std::underlying_type_t<T> to_underlying(T &value) {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    inline float random_range_float(float min, float max) {
        return min + (max - min) * roll(engine);
    }

    inline int random_range(int min, int max) {
        return min + (max - min) * int_roll(engine);
    }

    struct Potato {
        int count = 0;
    };

    struct Target {
        System::ECS::pid entity;
    };

    constexpr auto max_entities = 32000;
    constexpr auto repetitions = 1000;

    using System::ECS::Query;

    inline std::chrono::nanoseconds delta_time = std::chrono::nanoseconds::zero();

    template<System::ECS::SyscallType T>
    void potato_system([[maybe_unused]] T &syscall, Query<Potato, Target> &q) {
        for (auto &[id, comps]: q) {
            auto &target = comps.get<Target>().entity;
            auto &target_potato = syscall.template query<Potato>(target);
            // ++target_potato.count;
            // --comps.get<Potato>().count;
            // target = (target + 1) % max_entities;
        }
    }

    using RMtype = System::ECS::ResourceManager<max_entities, Potato, Target>;
    using SCtype = System::ECS::Syscall<max_entities, Potato, Target>;
    using TMtype = System::ECS::TaskManager<RMtype, SCtype, potato_system<SCtype> >;

    inline std::chrono::nanoseconds measure_and_log_execution_time(TMtype &task_manager) {
        // start clock
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // systems
        task_manager.run_all();

        // end clock
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        return delta_time;
    }

    inline void setup(TMtype &task_manager) {
        std::vector<System::ECS::pid> numbers;

        for (uint32_t i = 0; i < max_entities; ++i) {
            System::ECS::pid id = task_manager.create_entity();
            task_manager.add_component<Potato>(id, Potato{random_range(10, 15)});
            numbers.push_back(id);
        }

        std::ranges::shuffle(numbers, engine);

        for (uint32_t i = 0; i < max_entities; ++i) {
            const System::ECS::pid id = numbers.at(i);
            task_manager.add_component<Target>(id, Target{});
        }
    }

    inline int test() {
        TMtype task_manager;

        setup(task_manager);

        std::array<std::chrono::nanoseconds, repetitions> execution_times{};

        for (int i = 0; i < repetitions; ++i) {
            measure_and_log_execution_time(task_manager);
        }

        for (int i = 0; i < repetitions; ++i) {
            execution_times.at(i) = measure_and_log_execution_time(task_manager);
        }

        // write time elapsed to log file
        if (std::ofstream log_file("test3_kawa.log", std::ios::app); log_file.is_open()) {
            log_file << "Entity Count: " << max_entities << std::endl;
            log_file << "Repetitions: " << repetitions << std::endl;
            const auto total_time = std::accumulate(execution_times.begin(), execution_times.end(),
                                                    std::chrono::nanoseconds::zero());
            log_file << "Average: " << total_time.count() / static_cast<long double>(repetitions) << "ns" << std::endl;
            std::cout << "Average: " << total_time.count() / static_cast<long double>(repetitions) << "ns" << std::endl;
            log_file.close();
        }

        return 0;
    }
}
