cmake_minimum_required (VERSION 2.6)
project (bench)

# Download and unpack googletest at configure time
# configure_file(get-google-test.cmake googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
# add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
#                  ${CMAKE_BINARY_DIR}/googletest-build)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/google-test)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(GLOB SRC "src/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(ilp ${SRC})
set(COMMON_FLAGS "-Wall -Werror -std=c++11 -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_FLAGS} -O2")

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
add_custom_target(tidy)
foreach(SrcFile ${ALL_SRC})
  add_custom_command(TARGET tidy COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} -checks='-*,llvm-namespace-comment' -fix -fix-errors -header-filter='.*${PROJECT_SOURCE_DIR}/include.*' ${SrcFile})
endforeach()

# clang-format target
add_custom_target(format)
file(GLOB_RECURSE ALL_HDR "include" "*.hpp")
add_custom_command(TARGET format COMMAND clang-format -i ${ALL_SRC} ${ALL_HDR})
