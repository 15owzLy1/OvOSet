cmake_minimum_required(VERSION 3.22)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests/benchmark/skip_list SK_TEST)

# 创建测试可执行文件
add_executable(BM_SkipListTest ${SK_TEST})

# 链接数据结构库和Google Benchmark库
target_link_libraries(BM_SkipListTest data_structures benchmark::benchmark)

# 单独的 test
add_executable(BM_SKInsertContainsTest sk_insert_contains_test.cpp)
target_link_libraries(BM_SKInsertContainsTest data_structures benchmark::benchmark)
