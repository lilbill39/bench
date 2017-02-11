cmake_minimum_required (VERSION 2.6)
project (bench)
file(GLOB SRC "src/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(ilp ${SRC})

file(GLOB TEST_SRC "test/*.cpp")
add_executable(ilp_test ${TEST_SRC})
enable_testing()
add_test(NAME ilp_test COMMAND ilp_test)
target_link_libraries(ilp_test ilp)

add_definitions("-Wall -Werror -std=c++11")
