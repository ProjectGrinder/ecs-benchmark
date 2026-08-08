#pragma once

#include "entt/entt.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

namespace Test::Test4 {
    constexpr auto max_entities = 8000;
    constexpr auto repetitions = 1000;
    // global random engine
    inline std::random_device r;
    inline std::default_random_engine engine(r());
    inline std::uniform_real_distribution roll(0.0f, 1.0f);

    inline float random_range_float(float min, float max) {
        return min + (max - min) * roll(engine);
    }

    struct Step {
        int sum;
        int step;
    };

    struct Bit0 {};
    struct Bit1 {};
    struct Bit2 {};
    struct Bit3 {};
    struct Bit4 {};
    struct Bit5 {};
    struct Bit6 {};
    struct Bit7 {};

    inline void counting_system(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, Step &step) {
            step.sum = (step.sum + step.step) % 256;
            if (step.sum & 1 && !registry.any_of<Bit0>(entity)) {registry.emplace<Bit0>(entity);}
            if (step.sum & 2 && !registry.any_of<Bit1>(entity)) {registry.emplace<Bit1>(entity);}
            if (step.sum & 4 && !registry.any_of<Bit2>(entity)) {registry.emplace<Bit2>(entity);}
            if (step.sum & 8 && !registry.any_of<Bit3>(entity)) {registry.emplace<Bit3>(entity);}
            if (step.sum & 16 && !registry.any_of<Bit4>(entity)) {registry.emplace<Bit4>(entity);}
            if (step.sum & 32 && !registry.any_of<Bit5>(entity)) {registry.emplace<Bit5>(entity);}
            if (step.sum & 64 && !registry.any_of<Bit6>(entity)) {registry.emplace<Bit6>(entity);}
            if (step.sum & 128 && !registry.any_of<Bit7>(entity)) {registry.emplace<Bit7>(entity);}

            if (!(step.sum & 1) && registry.any_of<Bit0>(entity)) {registry.erase<Bit0>(entity);}
            if (!(step.sum & 2) && registry.any_of<Bit1>(entity)) {registry.erase<Bit1>(entity);}
            if (!(step.sum & 4) && registry.any_of<Bit2>(entity)) {registry.erase<Bit2>(entity);}
            if (!(step.sum & 8) && registry.any_of<Bit3>(entity)) {registry.erase<Bit3>(entity);}
            if (!(step.sum & 16) && registry.any_of<Bit4>(entity)) {registry.erase<Bit4>(entity);}
            if (!(step.sum & 32) && registry.any_of<Bit5>(entity)) {registry.erase<Bit5>(entity);}
            if (!(step.sum & 64) && registry.any_of<Bit6>(entity)) {registry.erase<Bit6>(entity);}
            if (!(step.sum & 128) && registry.any_of<Bit7>(entity)) {registry.erase<Bit7>(entity);}
        });
    }

    inline void counting_system_separated_add(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, Step &step) {
            step.sum = (step.sum + step.step) % 256;
        });
    }

    inline void counting_system_separated_0(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 1 && !registry.any_of<Bit0>(entity)) {registry.emplace<Bit0>(entity);}
            if (!(step.sum & 1) && registry.any_of<Bit0>(entity)) {registry.erase<Bit0>(entity);}
        });
    }

    inline void counting_system_separated_1(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 2 && !registry.any_of<Bit1>(entity)) {registry.emplace<Bit1>(entity);}
            if (!(step.sum & 2) && registry.any_of<Bit1>(entity)) {registry.erase<Bit1>(entity);}
        });
    }

    inline void counting_system_separated_2(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 4 && !registry.any_of<Bit2>(entity)) {registry.emplace<Bit2>(entity);}
            if (!(step.sum & 4) && registry.any_of<Bit2>(entity)) {registry.erase<Bit2>(entity);}
        });
    }

    inline void counting_system_separated_3(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 8 && !registry.any_of<Bit3>(entity)) {registry.emplace<Bit3>(entity);}
            if (!(step.sum & 8) && registry.any_of<Bit3>(entity)) {registry.erase<Bit3>(entity);}
        });
    }

    inline void counting_system_separated_4(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 16 && !registry.any_of<Bit4>(entity)) {registry.emplace<Bit4>(entity);}
            if (!(step.sum & 16) && registry.any_of<Bit4>(entity)) {registry.erase<Bit4>(entity);}
        });
    }

    inline void counting_system_separated_5(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 32 && !registry.any_of<Bit5>(entity)) {registry.emplace<Bit5>(entity);}
            if (!(step.sum & 32) && registry.any_of<Bit5>(entity)) {registry.erase<Bit5>(entity);}
        });
    }

    inline void counting_system_separated_6(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 64 && !registry.any_of<Bit6>(entity)) {registry.emplace<Bit6>(entity);}
            if (!(step.sum & 64) && registry.any_of<Bit6>(entity)) {registry.erase<Bit6>(entity);}
        });
    }

    inline void counting_system_separated_7(entt::registry &registry) {
        auto view = registry.view<Step>();
        view.each([&registry](auto entity, const Step &step) {
            if (step.sum & 128 && !registry.any_of<Bit7>(entity)) {registry.emplace<Bit7>(entity);}
            if (!(step.sum & 128) && registry.any_of<Bit7>(entity)) {registry.erase<Bit7>(entity);}
        });
    }

    using precision_type = std::chrono::nanoseconds;

    inline precision_type delta_time = precision_type::zero();

    inline precision_type update(entt::registry &registry) {
        // start clock
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // systems
        counting_system(registry);

        // end clock
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        delta_time = std::chrono::duration_cast<precision_type>(end - start);

        return delta_time;
    }

    inline void setup(entt::registry &registry) {
        for (int i = 0; i < max_entities; ++i) {
            auto id = registry.create();
            registry.emplace<Step>(id, Step{.sum = 0, .step = i});
        }
    }

    inline int test() {
        entt::registry registry;

        setup(registry);

        std::array<precision_type, repetitions> execution_times{};

        // blank run
        // while (true) {
        for (int i = 0; i < repetitions; i++) {
            update(registry);
        }

        // measure run
        for (int i = 0; i < repetitions; i++) {
            execution_times.at(i) = update(registry);
        }

        // write time elapsed to log file
        std::ofstream log_file("test4_entt.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "Max Entities: " << max_entities << std::endl;
            std::cout << "Max Entities: " << max_entities << std::endl;
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
