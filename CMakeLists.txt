cmake_minimum_required(VERSION 3.27)
project(sorting_benchmark)

set(CMAKE_CXX_STANDARD 23)

add_executable(sorting_benchmark
        main.cpp)
MATH(EXPR stack_size "32 * 1024 * 1024") # 32 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")