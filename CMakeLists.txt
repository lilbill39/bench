cmake_minimum_required (VERSION 2.6)
project (bench)
add_executable(bench bench.cpp)

add_definitions("-Wall -Werror -std=c++11")
