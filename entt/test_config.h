#pragma once
#include "entt/entt.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

namespace Test {
    // configurations
    constexpr auto repetitions = 1000;

    namespace Test1 {
        inline int test();
        constexpr auto max_entities = 1000;
    }

    namespace Test2 {
        inline int test();
        constexpr auto max_entities = 32000;
        constexpr auto strange_ratio = 0.20;
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
    namespace Bench = Test1;

    inline int test() {
        return Bench::test();
    }

}