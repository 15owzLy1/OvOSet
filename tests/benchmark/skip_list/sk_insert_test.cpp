#include <sys/resource.h>
#include "skip_list.h"
#include "benchmark/benchmark.h"

class SKInsertTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0);

        data.resize(size);
        Random rnd;
        for (int i = 0; i < size; ++i) {
            data[i] = rnd();
        }
        set = new SkipList<uint64_t, std::less<>>;
    }

    void TearDown(const ::benchmark::State& state) override {
        delete set;
    }

    std::vector<uint64_t> data;
    OvOSet<uint64_t, std::less<>>* set;
    size_t size;
};

BENCHMARK_DEFINE_F(SKInsertTest, PureInsert)(benchmark::State& state) {

    for (auto _ : state) {
        for (int i = 0; i < size; ++i) {
            set->Insert(data[i]);
        }
    }
    state.counters["MemoryUsage"] = benchmark::Counter(
            static_cast<double>(set->memory_usage_),
            benchmark::Counter::kDefaults,
            benchmark::Counter::kIs1024
    );
}

BENCHMARK_REGISTER_F(SKInsertTest, PureInsert)
        ->ArgsProduct({
                              {10000, 1000000, 5000000},
                      })
        ->Iterations(10)
        ->ArgNames({"Size"})
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();