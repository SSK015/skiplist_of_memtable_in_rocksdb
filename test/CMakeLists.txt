cmake_minimum_required(VERSION 3.10)

project(ibv_test)

# 指定编译器为g++
# set(CMAKE_CXX_COMPILER g++)

# 设置编译选项
# set(CMAKE_CXX_FLAGS "-std=c++11")
# set(CMAKE_CXX_STANDARD 17)


# include_directories(../include)
add_compile_options(-g)
# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../include)

# 添加源文件
add_executable(ibv_test ibv_test.c setup_ibv.c ibv_print_info.c)

# 链接库文件
target_link_libraries(ibv_test ibverbs)

