cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置项目名称
project(OvOSkipList)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(third/benchmark)

# 添加子目录
add_subdirectory(src)
add_subdirectory(tests)
