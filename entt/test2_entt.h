#pragma once

#include "entt/entt.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

namespace Test::Test2 {
    constexpr auto max_entities = 32000;
    constexpr auto repetitions = 1000;
    constexpr auto strange_ratio = 0.20;
    // global random engine
    inline std::random_device r;
    inline std::default_random_engine engine(r());
    inline std::uniform_real_distribution roll(0.0f, 1.0f);

    inline float random_range_float(float min, float max) {
        return min + (max - min) * roll(engine);
    }

    struct Position {
        float x, y;
    };

    struct Strangeness {
    };

    using precision_type = std::chrono::nanoseconds;

    inline precision_type delta_time = precision_type::zero();

    inline void strange_system(entt::registry &registry) {
        auto view = registry.view<Position, Strangeness>();

        view.each([](const auto entity, Position &pos) {
            const auto x = pos.x;
            const auto y = pos.y;
            pos.x = y;
            pos.y = -1 * x;
        });
    }

    inline precision_type update(entt::registry &registry) {
        // start clock
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // systems
        strange_system(registry);

        // end clock
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        delta_time = std::chrono::duration_cast<precision_type>(end - start);

        return delta_time;
    }

    inline void setup(entt::registry &registry) {
        std::vector<entt::entity> numbers;

        for (unsigned long long i = 0; i < max_entities; i++) {
            auto entity = registry.create();
            registry.emplace<Position>(entity, Position{random_range_float(-100, 100), random_range_float(-100, 100)});
            numbers.push_back(entity);
        }

        std::ranges::shuffle(numbers, engine);
        const uint32_t strange_count = floor(strange_ratio * max_entities);
        for (unsigned long long i = 0; i < strange_count; i++) {
            registry.emplace<Strangeness>(numbers.at(i));
        }
    }


    inline int test() {
        entt::registry registry;

        setup(registry);

        std::array<precision_type, repetitions> execution_times{};

        // blank run
        while (true) {
        // for (int i = 0; i < repetitions; i++) {
            update(registry);
        }

        // measure run
        for (int i = 0; i < repetitions; i++) {
            execution_times.at(i) = update(registry);
        }

        // write time elapsed to log file
        std::ofstream log_file("test2_entt.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "Strange Ratio: " << strange_ratio << std::endl;
            std::cout << "Strange Ratio: " << strange_ratio << std::endl;
            log_file << "Repetitions: " << repetitions << std::endl;
            std::cout << "Repetitions: " << repetitions << std::endl;

            auto total_time = std::accumulate(execution_times.begin(), execution_times.end(),
                                              precision_type::zero());

            std::string precision_name = "us";
            if constexpr (std::is_same_v<precision_type, std::chrono::nanoseconds>) {
                precision_name = "ns";
            }

            for (int i = 0; i < repetitions; i++) {
                log_file << execution_times.at(i).count() << precision_name << std::endl;
            }

            // Mean
            auto mean = total_time.count() / static_cast<long double>(repetitions);
            log_file << "Average: " << mean << precision_name << std::endl;
            std::cout << "Average: " << mean << precision_name << std::endl;

            // Standard Deviation
            long double variance = 0;
            for (int i = 0; i < repetitions; i++) {
                variance += (execution_times.at(i).count() - mean) * (execution_times.at(i).count() - mean);
            }
            auto std_dev = std::sqrt(variance / static_cast<long double>(repetitions));
            log_file << "Standard Deviation: " << std_dev << precision_name << std::endl;
            std::cout << "Standard Deviation: " << std_dev << precision_name << std::endl;

            // Min Time / Max Time
            auto min_time = std::numeric_limits<long double>::max();
            auto max_time = std::numeric_limits<long double>::min();
            for (int i = 0; i < repetitions; i++) {
                min_time = std::min(min_time, (long double) execution_times.at(i).count());
                max_time = std::max(max_time, (long double) execution_times.at(i).count());
            }
            log_file << "Min Time: " << min_time << precision_name << std::endl;
            std::cout << "Min Time: " << min_time << precision_name << std::endl;
            log_file << "Max Time: " << max_time << precision_name << std::endl;
            std::cout << "Max Time: " << max_time << precision_name << std::endl;

            log_file.close();
        }
        return 0;
    }
}
