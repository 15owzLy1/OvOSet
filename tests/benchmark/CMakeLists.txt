cmake_minimum_required(VERSION 3.22)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests/benchmark BM_TEST_FILES)

# 创建测试可执行文件
add_executable(BenchmarkTest ${BM_TEST_FILES})

# 链接数据结构库和Google Benchmark库
target_link_libraries(BenchmarkTest data_structures benchmark::benchmark)
