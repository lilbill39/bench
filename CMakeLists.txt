cmake_minimum_required (VERSION 2.6)
project (bench)
file(GLOB SRC "src/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(ilp ${SRC})

file(GLOB TEST_SRC "test/*.cpp")
add_executable(ilp_test ${TEST_SRC})
enable_testing()
add_test(NAME ilp_test COMMAND ilp_test --gtest_color=yes)
set(GTEST_DIR "${PROJECT_SOURCE_DIR}/test/googletest-release-1.8.0/googlemock/gtest")
add_library(gtest STATIC IMPORTED)
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${GTEST_DIR}/libgtest.a")
add_library(gtest_main STATIC IMPORTED)
set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION "${GTEST_DIR}/libgtest_main.a")
target_link_libraries(ilp_test ilp gtest gtest_main)
target_include_directories(ilp_test PRIVATE "${PROJECT_SOURCE_DIR}/test/googletest-release-1.8.0/googletest/include")
target_compile_options(ilp_test PUBLIC "-pthread")
set_target_properties(ilp_test PROPERTIES LINK_FLAGS "-pthread ${LINK_FLAGS}")

add_definitions("-O2 -Wall -Werror -std=c++11")

add_custom_target(check COMMAND ilp_test)
