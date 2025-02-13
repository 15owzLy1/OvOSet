#include <sys/resource.h>
#include "hash_table.h"
#include "rb_tree.h"
#include "skip_list.h"
#include "benchmark/benchmark.h"

enum class DS {
    HashTable = 0,
    SkipList = 1,
    RBTree = 2
};

class uint64Test : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0);
        int ds_type = state.range(2);

        data.resize(size);
        Random rnd;
        for (int i = 0; i < size; ++i) {
            data[i] = rnd();
        }

        switch (ds_type) {
            case 0:
                set = new HashTable<uint64_t, std::less<>>();
                break;
            case 1:
                set = new SkipList<uint64_t, std::less<>>();
                break;
            default:
                throw std::invalid_argument("Invalid DS type");
        }
    }

    void TearDown(const ::benchmark::State& state) override {
        delete set;
    }

    std::vector<uint64_t> data;
    OvOSet<uint64_t, std::less<>>* set;
    size_t size;
};

BENCHMARK_DEFINE_F(uint64Test, MixedOperations)(benchmark::State& state) {
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

BENCHMARK_REGISTER_F(uint64Test, MixedOperations)
        ->ArgsProduct({
                              {10000, 1000000, 10000000},
                              {50, 20, 10},
                              {0, 1}
                      })
        ->ArgNames({"Size", "InsertRate", "DSType"});

BENCHMARK_MAIN();