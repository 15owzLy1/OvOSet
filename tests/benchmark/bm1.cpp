// tests/benchmark.cpp
#include <sys/resource.h>
#include "hash_table.h"
#include "rb_tree.h"
#include "skip_list.h"
#include "benchmark/benchmark.h"

static void BM_HashTableInsert(benchmark::State& state) {
    for (auto _ : state) {
        rusage usage{};
        HashTable<int,std::less<>> hashTable;
        for (int i = 0; i < state.range(0); ++i) {
            hashTable.Insert(i);
        }
        // 获取内存使用
        getrusage(RUSAGE_SELF, &usage);
        state.counters["Memory"] = static_cast<double>(usage.ru_maxrss); // 以 KB 为单位
    }
}

static void BM_RBTreeInsert(benchmark::State& state) {
    for (auto _ : state) {
        rusage usage{};
        RBTree<int,std::less<>> rbTree;
        for (int i = 0; i < state.range(0); ++i) {
//            rbTree.Insert(i);
        }
        getrusage(RUSAGE_SELF, &usage);
        state.counters["Memory"] = static_cast<double>(usage.ru_maxrss); // 以 KB 为单位
    }
}

static void BM_SkipListInsert(benchmark::State& state) {
    for (auto _ : state) {
        rusage usage{};
        SkipList<int,std::less<>> skipList;
        for (int i = 0; i < state.range(0); ++i) {
            skipList.Insert(i);
        }
        getrusage(RUSAGE_SELF, &usage);
        state.counters["Memory"] = static_cast<double>(usage.ru_maxrss); // 以 KB 为单位
    }
}

BENCHMARK(BM_HashTableInsert)->Range(8, 8<<10);
BENCHMARK(BM_SkipListInsert)->Range(8, 8<<10);
BENCHMARK(BM_RBTreeInsert)->Range(8, 8<<10);

BENCHMARK_MAIN();
