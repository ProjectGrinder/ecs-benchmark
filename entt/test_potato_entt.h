#pragma once

#include "entt/entt.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

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
        entt::entity entity;
    };

    constexpr auto max_entities = 2000;
    constexpr auto repetitions = 1000;

    inline std::chrono::nanoseconds delta_time = std::chrono::nanoseconds::zero();

    inline void potato_system(entt::registry &registry) {
        auto view = registry.view<Potato, Target>();

        view.each([&registry](const auto entity, Potato &potato, Target &target) {
            const auto &to = target.entity;
            auto &[to_potato] = registry.get<Potato>(to);
            to_potato++;
            potato.count--;
            target.entity = static_cast<entt::entity>((1 + to_underlying(target.entity)) % max_entities);
        });
    }

    inline std::chrono::nanoseconds update(entt::registry &registry) {
        // start clock
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // systems
        potato_system(registry);

        // end clock
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        return delta_time;
    }

    inline void setup(entt::registry &registry) {
        std::vector<entt::entity> numbers;

        for (unsigned long long i = 0; i < max_entities; i++) {
            auto entity = registry.create();
            registry.emplace<Potato>(entity, Potato{random_range(10, 15)});
            numbers.push_back(entity);
        }

        std::ranges::shuffle(numbers, engine);
        for (unsigned long long i = 0; i < max_entities; i++) {
            registry.emplace<Target>(static_cast<entt::entity>(i), numbers.at(i));
        }
    }

    inline int test() {
        entt::registry registry;

        setup(registry);

        std::array<std::chrono::nanoseconds, repetitions> execution_times{};

        // blank run
        for (int i = 0; i < repetitions; i++) {
            update(registry);
        }

        // measure run
        for (int i = 0; i < repetitions; i++) {
            execution_times.at(i) = update(registry);
        }

        // write time elapsed to log file
        std::ofstream log_file("test3_entt.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "Max Entities: " << max_entities << std::endl;
            log_file << "Repetitions: " << repetitions << std::endl;

            auto total_time = std::accumulate(execution_times.begin(), execution_times.end(),
                                              std::chrono::nanoseconds::zero());
            log_file << "Average: " << total_time.count() / static_cast<long double>(repetitions) << "ns" << std::endl;
            std::cout << "Average: " << total_time.count() / static_cast<long double>(repetitions) << "ns" << std::endl;

            log_file.close();
        }
        return 0;
    }
}
