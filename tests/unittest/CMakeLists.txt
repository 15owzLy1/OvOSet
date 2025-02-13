cmake_minimum_required(VERSION 3.22)

# 收集所有测试源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/tests/unittest UNIT_TEST_FILES)

# 添加单元测试可执行文件
add_executable(UnitTests ${UNIT_TEST_FILES})



add_executable(HashTableIOTest hashtable_io_test.cpp)
target_link_libraries(HashTableIOTest data_structures gtest gtest_main)

add_executable(SkipListIOTest skiplist_io_test.cpp)
target_link_libraries(SkipListIOTest data_structures gtest gtest_main)

add_executable(RBTreeIOTest rb_tree_io_test.cpp)
target_link_libraries(RBTreeIOTest data_structures gtest gtest_main)


# 链接 gtest 和 gtest_main
target_link_libraries(UnitTests gtest gtest_main)
