cmake_minimum_required(VERSION 3.22)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests/benchmark/rb_tree RB_TEST)

# 创建测试可执行文件
add_executable(BM_RBTreeTest ${RB_TEST})

# 链接数据结构库和Google Benchmark库
target_link_libraries(BM_RBTreeTest data_structures benchmark::benchmark)

# 单独的 test
add_executable(BM_RBInsertContainsTest rb_insert_contains_test.cpp)
target_link_libraries(BM_RBInsertContainsTest data_structures benchmark::benchmark)

add_executable(BM_RBInsertTest rb_insert_test.cpp)
target_link_libraries(BM_RBInsertTest data_structures benchmark::benchmark)

add_executable(BM_RBRangeTest rb_range_test.cpp)
target_link_libraries(BM_RBRangeTest data_structures benchmark::benchmark)

add_executable(BM_RBMultiRangeTest rb_multi_range_test.cpp)
target_link_libraries(BM_RBMultiRangeTest data_structures benchmark::benchmark)
