#pragma once

#include "test_config.h"

namespace Test::Test4 {

    // global random engine
    inline std::random_device r;
    inline std::default_random_engine engine(r());
    inline std::uniform_real_distribution roll(0.0f, 1.0f);

    inline float random_range_float(const float min, const float max) {
        return min + (max - min) * roll(engine);
    }

    using System::ECS::Query;

    inline precision_type delta_time = precision_type::zero();

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

    template<System::ECS::SyscallType T>
    void counting_system(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            sum = (sum + step) % 256;
            if (sum & 1 && !syscall.template has_component<Bit0>(id)) {syscall.template add_component<Bit0>(id, {});}
            if (sum & 2 && !syscall.template has_component<Bit1>(id)) {syscall.template add_component<Bit1>(id, {});}
            if (sum & 4 && !syscall.template has_component<Bit2>(id)) {syscall.template add_component<Bit2>(id, {});}
            if (sum & 8 && !syscall.template has_component<Bit3>(id)) {syscall.template add_component<Bit3>(id, {});}
            if (sum & 16 && !syscall.template has_component<Bit4>(id)) {syscall.template add_component<Bit4>(id, {});}
            if (sum & 32 && !syscall.template has_component<Bit5>(id)) {syscall.template add_component<Bit5>(id, {});}
            if (sum & 64 && !syscall.template has_component<Bit6>(id)) {syscall.template add_component<Bit6>(id, {});}
            if (sum & 128 && !syscall.template has_component<Bit7>(id)) {syscall.template add_component<Bit7>(id, {});}

            if (!(sum & 1) && syscall.template has_component<Bit0>(id)) {syscall.template remove_component<Bit0>(id);}
            if (!(sum & 2) && syscall.template has_component<Bit1>(id)) {syscall.template remove_component<Bit1>(id);}
            if (!(sum & 4) && syscall.template has_component<Bit2>(id)) {syscall.template remove_component<Bit2>(id);}
            if (!(sum & 8) && syscall.template has_component<Bit3>(id)) {syscall.template remove_component<Bit3>(id);}
            if (!(sum & 16) && syscall.template has_component<Bit4>(id)) {syscall.template remove_component<Bit4>(id);}
            if (!(sum & 32) && syscall.template has_component<Bit5>(id)) {syscall.template remove_component<Bit5>(id);}
            if (!(sum & 64) && syscall.template has_component<Bit6>(id)) {syscall.template remove_component<Bit6>(id);}
            if (!(sum & 128) && syscall.template has_component<Bit7>(id)) {syscall.template remove_component<Bit7>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_add(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            sum = (sum + step) % 256;
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_0(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 1 && !syscall.template has_component<Bit0>(id)) {syscall.template add_component<Bit0>(id, {});}

            if (!(sum & 1) && syscall.template has_component<Bit0>(id)) {syscall.template remove_component<Bit0>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_1(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 2 && !syscall.template has_component<Bit1>(id)) {syscall.template add_component<Bit1>(id, {});}

            if (!(sum & 2) && syscall.template has_component<Bit1>(id)) {syscall.template remove_component<Bit1>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_2(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 4 && !syscall.template has_component<Bit2>(id)) {syscall.template add_component<Bit2>(id, {});}

            if (!(sum & 4) && syscall.template has_component<Bit2>(id)) {syscall.template remove_component<Bit2>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_3(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 8 && !syscall.template has_component<Bit3>(id)) {syscall.template add_component<Bit3>(id, {});}

            if (!(sum & 8) && syscall.template has_component<Bit3>(id)) {syscall.template remove_component<Bit3>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_4(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 16 && !syscall.template has_component<Bit4>(id)) {syscall.template add_component<Bit4>(id, {});}

            if (!(sum & 16) && syscall.template has_component<Bit4>(id)) {syscall.template remove_component<Bit4>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_5(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 32 && !syscall.template has_component<Bit5>(id)) {syscall.template add_component<Bit5>(id, {});}

            if (!(sum & 32) && syscall.template has_component<Bit5>(id)) {syscall.template remove_component<Bit5>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_6(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 64 && !syscall.template has_component<Bit6>(id)) {syscall.template add_component<Bit6>(id, {});}

            if (!(sum & 64) && syscall.template has_component<Bit6>(id)) {syscall.template remove_component<Bit6>(id);}
        }
    }

    template<System::ECS::SyscallType T>
    void counting_system_separated_7(T &syscall, Query<Step> &query) {
        for (auto &[id, comps] : query) {
            auto &[sum, step] = comps.get<Step>();
            if (sum & 128 && !syscall.template has_component<Bit7>(id)) {syscall.template add_component<Bit7>(id, {});}

            if (!(sum & 128) && syscall.template has_component<Bit7>(id)) {syscall.template remove_component<Bit7>(id);}
        }
    }

    // metaprogramming to compile-time create ResourceManager based on ComponentTuple
    template<size_t N, typename T>
    struct make_ecs_types;

    template<size_t N, typename... Components>
    struct make_ecs_types<N, std::tuple<Components...>>
    {
        using RMtype = System::ECS::ResourceManager<N, Components...>;
        using SCtype = System::ECS::Syscall<N, Components...>;
    };

    template<size_t MaxResource, typename CTuple>
    using make_resource_manager_t = make_ecs_types<MaxResource, CTuple>::RMtype;

    template<size_t MaxResource, typename CTuple>
    using make_syscall_t = make_ecs_types<MaxResource, CTuple>::SCtype;

    using ComponentTuple = std::tuple<Step, Bit0, Bit1, Bit2, Bit3, Bit4, Bit5, Bit6, Bit7>;

    using RMtype = make_resource_manager_t<max_entities, ComponentTuple>;
    using SCtype = make_syscall_t<max_entities, ComponentTuple>;
    using TMtype = System::ECS::TaskManager<RMtype, SCtype, counting_system<SCtype>>;

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

    inline void setup(TMtype &task_manager) {
        for (int i = 0; i < max_entities; ++i) {
            task_manager.create_entity(Step{.sum = 0, .step = i});
        }
    }

    inline int test() {
        TMtype task_manager;

        setup(task_manager);

        std::array<precision_type, repetitions> execution_times{};

        // while (true) {
        for (int i = 0; i < repetitions; ++i) {
            measure_and_log_execution_time(task_manager);
        }


        for (int i = 0; i < repetitions; ++i) {
            execution_times.at(i) = measure_and_log_execution_time(task_manager);
        }

        // write time elapsed to log file
        std::ofstream log_file("test4_kawa.log", std::ios::app);
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

            log_file.close();
        }
        return 0;
    }
}
