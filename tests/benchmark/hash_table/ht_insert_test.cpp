#include <sys/resource.h>
#include "hash_table.h"
#include "benchmark/benchmark.h"

class HTInsertTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0);

        data.resize(size);
        Random rnd;
        for (int i = 0; i < size; ++i) {
            data[i] = rnd();
        }
        set = new HashTable<uint64_t, std::less<>>;
    }

    void TearDown(const ::benchmark::State& state) override {
        delete set;
    }

    std::vector<uint64_t> data;
    OvOSet<uint64_t, std::less<>>* set;
    size_t size;
};

BENCHMARK_DEFINE_F(HTInsertTest, PureInsert)(benchmark::State& state) {

    for (auto _ : state) {
        for (int i = 0; i < size; ++i) {
            set->Insert(data[i]);
        }
        state.counters["MemoryUsage"] = benchmark::Counter(
                (double)set->memory_usage_ / 1024 / 1024
        );
    }
}

BENCHMARK_REGISTER_F(HTInsertTest, PureInsert)
        ->ArgsProduct({
                              {10000, 1000000, 5000000},
                      })
        ->ArgNames({"Size"})
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();