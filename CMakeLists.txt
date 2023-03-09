cmake_minimum_required(VERSION 3.10)
project(skiplist_of_memtable_in_rocksdb)


set(CMAKE_CXX_STANDARD 17)


include_directories(include)


add_executable(skiplist_of_memtable_in_rocksdb test/skiplist_test.cc)
target_link_libraries(skiplist_of_memtable_in_rocksdb gtest gtest_main -pthread)


target_link_libraries(skiplist_of_memtable_in_rocksdb rocksdb)
