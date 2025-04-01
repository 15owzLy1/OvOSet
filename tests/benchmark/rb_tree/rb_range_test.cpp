#include <sys/resource.h>
#include <algorithm>
#include "rb_tree.h"
#include "benchmark/benchmark.h"

class RBRangeTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0);

        data.resize(size);
        uint64_t current = 0;
        Random rnd;
        for (int i = 0; i < size; ++i) {
            current += rnd() % 1000 + 1;
            data[i] = current;
        }

        set = std::make_unique<RBTree<uint64_t, std::less<>>>();
        for (int i = 0; i < size; ++i) {
            set->Insert(data[i]);
        }

        auto len = rnd() % size;
        auto start = rnd() % (size - len);
        lower = data[start];
        upper = data[start + len];
    }

    void TearDown(const ::benchmark::State& state) override {
    }

    std::vector<uint64_t> data;
    std::unique_ptr<OvOSet<uint64_t, std::less<>>> set;
    size_t size;
    uint64_t lower, upper;
};

BENCHMARK_DEFINE_F(RBRangeTest, RangeQuery)(benchmark::State& state) {
    std::vector<uint64_t> result;
    result.reserve(size);

    for (auto _ : state) {
        result.clear();
        set->GetRange(lower, upper, &result);
    }

    // 添加性能计数器
    state.counters["MemoryUsage"] = benchmark::Counter(
            static_cast<double>(set->memory_usage_),
            benchmark::Counter::kDefaults,
            benchmark::Counter::kIs1024
    );
}

BENCHMARK_REGISTER_F(RBRangeTest, RangeQuery)
        ->ArgsProduct({{1000, 10000, 100000}})
        ->Iterations(50000)
        ->ArgNames({"Size"})
        ->Unit(benchmark::kMillisecond)
        ->Threads(1);

BENCHMARK_MAIN();