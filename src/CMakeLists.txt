cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集所有源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_FILES)

# 创建一个静态库
add_library(data_structures STATIC ${SRC_FILES})