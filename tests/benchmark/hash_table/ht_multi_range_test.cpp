#include <sys/resource.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <random>
#include "hash_table.h"
#include "benchmark/benchmark.h"

class HTMultiRangeTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0);

        data.resize(size);
        uint64_t current = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000);

        for (int i = 0; i < size; ++i) {
            current += dis(gen);
            data[i] = current;
        }

        set = std::make_unique<HashTable<uint64_t, std::less<>>>();
        for (int i = 0; i < size; ++i) {
            set->Insert(data[i]);
        }

        num_threads = 10;
        lower.reserve(num_threads * 100);
        upper.reserve(num_threads * 100);
        for (int i = 0; i < num_threads * 100; ++i) {
            auto len = size / 3 + gen() % (size / 3);
            auto start = gen() % (size - len);
            lower.push_back(data[start]);
            upper.push_back(data[start + len]);
        }
    }

    void TearDown(const ::benchmark::State& state) override {
    }

    std::vector<uint64_t> data;
    std::unique_ptr<OvOSet<uint64_t, std::less<>>> set;
    size_t size;
    int num_threads;
    std::vector<uint64_t> lower, upper;
};

BENCHMARK_DEFINE_F(HTMultiRangeTest, RangeQuery)(benchmark::State& state) {
    std::vector<std::thread> threads;
    std::vector<std::vector<uint64_t>> results(num_threads);

    for (auto _ : state) {
        // 启动多个线程执行范围查询
        threads.clear();
        for (int i = 0; i < num_threads; ++i) {
            results[i].clear();
            threads.emplace_back([this, &results, i] {
                for (int j = i * 100; j < (i + 1) * 100; ++j) {
                    set->GetRange(lower[j], upper[j], &results[i]);
                }
            });
        }

        // 等待所有线程完成
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // 合并结果并验证
        std::vector<uint64_t> merged_result;
        for (auto& res : results) {
            merged_result.insert(merged_result.end(), res.begin(), res.end());
        }
        merged_result.erase(std::unique(merged_result.begin(), merged_result.end()), merged_result.end());
    }
}

BENCHMARK_REGISTER_F(HTMultiRangeTest, RangeQuery)
        ->ArgsProduct({{1000, 10000, 100000}})
        ->Iterations(10)
        ->ArgNames({"Size"})
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
