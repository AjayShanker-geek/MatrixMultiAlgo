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
    src/common/benchmark.cpp
    src/common/print_out.cpp
    src/common/validation.cpp
)

target_include_directories(matrix_common
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(matrix_common
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(naive_matmul
    src/naive/main.cpp
    src/naive/naive.cpp
)

target_link_libraries(naive_matmul
    PRIVATE
        matrix_common
)

add_executable(cache_aware_matmul
    src/cache_aware/main.cpp
    src/cache_aware/cache_aware.cpp
)

target_link_libraries(cache_aware_matmul
    PRIVATE
        matrix_common
)

add_executable(cache_oblivious_matmul
    src/cache_oblivious/main.cpp
    src/cache_oblivious/cache_oblivious.cpp
)

target_link_libraries(cache_oblivious_matmul
    PRIVATE
        matrix_common
)


