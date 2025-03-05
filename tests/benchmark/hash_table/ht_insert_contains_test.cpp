#include <sys/resource.h>
#include "hash_table.h"
#include "benchmark/benchmark.h"

class HTInsertContainsTest : public benchmark::Fixture {
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

BENCHMARK_DEFINE_F(HTInsertContainsTest, MixedOperations)(benchmark::State& state) {
    const int insert_rate = state.range(1);

    Random op_rnd;

    for (auto _ : state) {
        for (int i = 0; i < size; ++i) {
            if (op_rnd() % 100 < insert_rate) {
                set->Insert(data[i]);
            } else {
                set->Contains(data[i]);
            }
        }
    }
}

BENCHMARK_REGISTER_F(HTInsertContainsTest, MixedOperations)
        ->ArgsProduct({
                              {10000, 1000000, 5000000},
                              {50, 20, 10},
                      })
        ->Iterations(10)
        ->ArgNames({"SizeInsertRate", "InsertRate"})
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();