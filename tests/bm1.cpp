// tests/benchmark.cpp
#include "hash_table.h"
#include "rb_tree.h"
#include "skip_list.h"
#include "benchmark/benchmark.h"

static void BM_HashTableInsert(benchmark::State& state) {
    HashTable hashTable;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
//            hashTable.Insert(i);
        }
    }
}

static void BM_RBTreeInsert(benchmark::State& state) {
    RBTree rbTree;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
//            rbTree.Insert(i);
        }
    }
}

static void BM_SkipListInsert(benchmark::State& state) {
    SkipList skipList;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
//            skipList.Insert(i);
        }
    }
}

BENCHMARK(BM_HashTableInsert)->Range(8, 8<<10);
BENCHMARK(BM_RBTreeInsert)->Range(8, 8<<10);
BENCHMARK(BM_SkipListInsert)->Range(8, 8<<10);

BENCHMARK_MAIN();
