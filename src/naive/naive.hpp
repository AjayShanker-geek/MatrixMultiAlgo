#pragma once

#include <array>
#include <cstddef>
#include <iostream>

#include "logger.hpp"
#include "param.hpp"
#include "timer.hpp"
#include "validation.hpp"

namespace naive {

template<std::size_t N>
void naive_matmul(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  std::array<std::array<double, N>, N> &C)
{
  // i-j-k loop order
  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) {
      C.at(i).at(j) = 0.0;

      for (std::size_t k = 0; k < N; ++k) { C.at(i).at(j) += A.at(i).at(k) * B.at(k).at(j); }
    }
  }
}

template<std::size_t N>
void run_benchmark(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  const std::array<std::array<double, N>, N> &C,
  timer::Timer &timer,
  Logger &logger)
{
  double elapsed_t{ 0.0 };

  for (std::size_t i = 0; i < LOOP_EXPERIMENTAL; ++i) {
    // create a unique_ptr to hold the result matrix
    auto result = std::make_unique<std::array<std::array<double, N>, N>>();

    // measure time taken
    timer.reset();
    naive_matmul(A, B, *result);
    elapsed_t = timer.elapsed_ms();

    std::cout << "[naive]" << N << " x " << N << (check_matrix_result(*result, C) ? " PASS" : " FAIL") << '\n';
    logger.log_timing("naive", N, elapsed_t, 0, 0.0);

    result.reset();// free memory
  }
}
}// namespace naive
