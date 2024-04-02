cmake_minimum_required(VERSION 3.26)
project(sorting_benchmark)

set(CMAKE_CXX_STANDARD 23)

add_executable(sorting_benchmark
        main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-stack_size,0x08000000")