cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests TEST_FILES)

# 创建测试可执行文件
add_executable(benchmark_tests ${TEST_FILES})

# 链接数据结构库和Google Benchmark库
target_link_libraries(benchmark_tests data_structures benchmark::benchmark)
