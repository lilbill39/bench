cmake_minimum_required (VERSION 2.6)
project (bench)

# Download and unpack googletest at configure time
configure_file(get-google-test.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(GLOB SRC "src/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(ilp ${SRC})
add_definitions("-O2 -Wall -Werror -std=c++11 -fno-omit-frame-pointer")

# Add tests
file(GLOB TEST_SRC "test/*.cpp")
add_executable(ilp_test ${TEST_SRC})
enable_testing()
add_test(NAME ilp_test COMMAND ilp_test --gtest_color=yes)
target_link_libraries(ilp_test ilp gtest gtest_main)

# make check just runs test
add_custom_target(check COMMAND ilp_test)

# Add benchmark exe
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/google-benchmark)
file(GLOB BENCHMARK_SRC "benchmark/*.cpp")
add_executable(ilp_bench ${BENCHMARK_SRC})
target_link_libraries(ilp_bench benchmark -pthread ilp)

# clang-tidy target
set(ALL_SRC ${SRC} ${TEST_SRC} ${BENCHMARK_SRC})
# find_program(TIDY clang-tidy)
# add_executable(clang-tidy IMPORTED)
# set_property(TARGET clang-tidy PROPERTY IMPORTED_LOCATION ${TIDY})
add_custom_target(tidy)
foreach(SrcFile ${ALL_SRC})
  add_custom_command(TARGET tidy COMMAND clang-tidy-3.9 -p ${CMAKE_BINARY_DIR} -checks='-*,llvm-namespace-comment' -fix -fix-errors -header-filter='.*${PROJECT_SOURCE_DIR}/include.*' ${SrcFile})
endforeach()

add_custom_target(format)
add_custom_command(TARGET format COMMAND clang-format-3.9 -i ${ALL_SRC})
