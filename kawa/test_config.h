#pragma once
#include "ecs.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

namespace Test {
    // configurations
    constexpr auto seed = 0;
    constexpr auto repetitions = 1000;
    using precision_type = std::chrono::nanoseconds;

    namespace Test1 {
        inline int test();
        constexpr auto max_entities = 64000;
    }

    namespace Test2 {
        inline int test();
        constexpr auto max_entities = 32000;
        constexpr auto strange_ratio = 0.0;
    }

    namespace Test3 {
        inline int test();
        constexpr auto entity_rate = 2048;
        constexpr auto max_entities = 3 * entity_rate;
    }

    namespace Test4 {
        inline int test();
        constexpr auto max_entities = 8000;
    }

    // modify bench to activate
    namespace Bench = Test4;

    inline int test() {
        return Bench::test();
    }

    inline std::array<precision_type, 3> min_median_max(std::array<precision_type, repetitions> &execution_times) {
        std::ranges::sort(execution_times);
        const precision_type min = execution_times.at(0);

        precision_type median = precision_type::zero();
        if (execution_times.size() % 2 == 0) {
            median = (execution_times.at(execution_times.size() / 2) + execution_times.at(execution_times.size() / 2 - 1)) / 2;
        }
        else {
            median = execution_times.at(execution_times.size() / 2);
        }

        const precision_type max = execution_times.at(execution_times.size() - 1);
        return { min, median, max };
    }

}