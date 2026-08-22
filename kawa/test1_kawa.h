#pragma once

#include "test_config.h"

namespace Test::Test1 {

    // global random engine
    inline std::default_random_engine engine(seed);
    inline std::uniform_real_distribution roll(0.0f, 1.0f);

    inline float random_range_float(const float min, const float max) {
        return min + (max - min) * roll(engine);
    }

    struct Position {
        float x, y;
    };

    struct Velocity {
        float x, y;
    };

    using System::ECS::Query;

    inline precision_type delta_time = precision_type::zero();

    template<typename T>
    void movement_system([[maybe_unused]] T &syscall, Query<Position, Velocity> &q) {
        const auto dt = static_cast<long double>(delta_time.count()) / 1000000.0L;
        for (auto &[id, comps]: q) {
            comps.get<Position>().x += comps.get<Velocity>().x * static_cast<float>(dt);
            comps.get<Position>().y += comps.get<Velocity>().y * static_cast<float>(dt);
        }
    }

    using RMtype = System::ECS::ResourceManager<max_entities, Position, Velocity>;
    using SCtype = System::ECS::Syscall<max_entities, Position, Velocity>;
    using TMtype = System::ECS::TaskManager<RMtype, SCtype, movement_system<SCtype> >;

    inline precision_type measure_and_log_execution_time(TMtype &task_manager) {
        // start clock
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // systems
        task_manager.run_all();

        // end clock
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        delta_time = std::chrono::duration_cast<precision_type>(end - start);

        return delta_time;
    }

    inline int test() {
        const auto initial_memory = process_memory_bytes();
        TMtype task_manager;

        for (uint32_t i = 0; i < max_entities; ++i) {
            const System::ECS::pid id = task_manager.create_entity();
            task_manager.add_component<Position>(id, {
                                                     random_range_float(0.0f, 1000.0f),
                                                     random_range_float(0.0f, 1000.0f)
                                                 });
            task_manager.add_component<Velocity>(id, {
                                                     random_range_float(-1.0f, 1.0f), random_range_float(-1.0f, 1.0f)
                                                 });
        }

        std::array<precision_type, repetitions> execution_times{};

        // while (true) {
        for (int i = 0; i < repetitions; ++i) {
            measure_and_log_execution_time(task_manager);
        }

        for (int i = 0; i < repetitions; ++i) {
            execution_times.at(i) = measure_and_log_execution_time(task_manager);
        }

        const auto final_memory = process_memory_bytes();
        const auto memory_used = final_memory >= initial_memory
                                      ? final_memory - initial_memory
                                      : 0;

        // write time elapsed to log file
        std::ofstream log_file("test1_kawa.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "Max Entities: " << max_entities << std::endl;
            std::cout << "Max Entities: " << max_entities << std::endl;
            log_file << "Repetitions: " << repetitions << std::endl;
            std::cout << "Repetitions: " << repetitions << std::endl;

            std::string precision_name = "us";
            if constexpr (std::is_same_v<precision_type, std::chrono::nanoseconds>) {
                precision_name = "ns";
            }

            for (int i = 0; i < repetitions; i++) {
                log_file << execution_times.at(i).count() << precision_name << std::endl;
            }

            // Min, Median, Max
            auto arr = min_median_max(execution_times);
            auto min = arr.at(0);
            auto median = arr.at(1);
            auto max = arr.at(2);

            // Mean
            auto total_time = std::accumulate(execution_times.begin(), execution_times.end(),
                                  precision_type::zero());
            auto mean = static_cast<long double>(total_time.count()) / static_cast<long double>(repetitions);

            // Standard Deviation
            long double variance = 0;
            for (int i = 0; i < repetitions; i++) {
                variance += (static_cast<long double>(execution_times.at(i).count()) - mean) * (static_cast<long double>(execution_times.at(i).count()) - mean);
            }
            auto std_dev = std::sqrt(variance / static_cast<long double>(repetitions));

            log_file << "Average: " << mean << precision_name << std::endl;
            std::cout << "Average: " << mean << precision_name << std::endl;
            log_file << "Median: " << median << precision_name << std::endl;
            std::cout << "Median: " << median << precision_name << std::endl;
            log_file << "Standard Deviation: " << std_dev << precision_name << std::endl;
            std::cout << "Standard Deviation: " << std_dev << precision_name << std::endl;

            log_file << "Min Time: " << min << precision_name << std::endl;
            std::cout << "Min Time: " << min << precision_name << std::endl;
            log_file << "Max Time: " << max << precision_name << std::endl;
            std::cout << "Max Time: " << max << precision_name << std::endl;

            log_file << "Memory Used: " << memory_used << " bytes ("
                     << static_cast<long double>(memory_used) / (1024.0L * 1024.0L)
                     << " MiB)" << std::endl;
            std::cout << "Memory Used: " << memory_used << " bytes ("
                      << static_cast<long double>(memory_used) / (1024.0L * 1024.0L)
                      << " MiB)" << std::endl;

            log_file.close();
        }
        return 0;
    }
}
