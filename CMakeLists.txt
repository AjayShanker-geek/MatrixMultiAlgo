cmake_minimum_required(VERSION 4.4.3)

project(matrix_cal
  VERSION 0.0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(matrix_common
    src/common/validation.cpp
)

# Ensure no optimzation done by the compiler
add_library(no_optimization INTERFACE)

target_compile_options(no_optimization
    INTERFACE
        # $<$<CONFIG:Debug>:-O0>
        # $<$<CONFIG:Debug>:-g>
        -O0 # Disable compiler optimizations
        -pg # Enable profiling with gprof
)

target_include_directories(matrix_common
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/matrix
)

# Add compiler flags for matrix_common
target_compile_options(matrix_common
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# Build naive matrix multiplication binary
add_executable(naive_matmul
    src/naive/main.cpp
    src/naive/naive.cpp
)

target_link_libraries(naive_matmul
    PRIVATE
        matrix_common
        no_optimization
)

# Build cache-aware matrix multiplication binary
add_executable(cache_aware_matmul
    src/cache_aware/main.cpp
    src/cache_aware/cache_aware.cpp
)

target_link_libraries(cache_aware_matmul
    PRIVATE
        matrix_common
        no_optimization
)

# Build cache-oblivious matrix multiplication binary
add_executable(cache_oblivious_matmul
    src/cache_oblivious/main.cpp
    src/cache_oblivious/cache_oblivious.cpp
)

target_link_libraries(cache_oblivious_matmul
    PRIVATE
        matrix_common
        no_optimization
)


