cmake_minimum_required(VERSION 3.22)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests/benchmark/hash_table HT_TEST)

# 创建测试可执行文件
add_executable(BM_HashTableTest ${HT_TEST})

# 链接数据结构库和Google Benchmark库
target_link_libraries(BM_HashTableTest data_structures benchmark::benchmark)

# 单独的 test
add_executable(BM_HTInsertContainsTest ht_insert_contains_test.cpp)
target_link_libraries(BM_HTInsertContainsTest data_structures benchmark::benchmark)

add_executable(BM_HTInsertTest ht_insert_test.cpp)
target_link_libraries(BM_HTInsertTest data_structures benchmark::benchmark)
