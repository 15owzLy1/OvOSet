cmake_minimum_required(VERSION 3.22)

# 添加子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/benchmark/hash_table)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/benchmark/rb_tree)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/benchmark/skip_list)
