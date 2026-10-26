#pragma once

#include <iostream>
#include <vector>

#include "logger.hpp"
#include "param.hpp"
#include "timer.hpp"
#include "validation.hpp"


namespace cache_aware {

const std::vector<int> t_arr = { 8, 16, 32, 64, 128 };

template<std::size_t N>
void blocked_matmul(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  std::array<std::array<double, N>, N> &C,
  std::size_t block_size)
{

  for (std::size_t ii = 0; ii < N; ii += block_size) {
    for (std::size_t jj = 0; jj < N; jj += block_size) {
      for (std::size_t kk = 0; kk < N; kk += block_size) {

        // Ensure block does not exceed N
        const std::size_t i_end = std::min(ii + block_size, N);
        const std::size_t j_end = std::min(jj + block_size, N);
        const std::size_t k_end = std::min(kk + block_size, N);

        for (std::size_t i = ii; i < i_end; ++i) {
          for (std::size_t j = jj; j < j_end; ++j) {
            for (std::size_t k = kk; k < k_end; ++k) { C[i][j] += A[i][k] * B[k][j]; }
          }
        }
      }
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

  for (int blk : t_arr) {
    for (std::size_t i = 0; i < LOOP_EXPERIMENTAL; ++i) {
      // create a unique_ptr to hold the result matrix
      auto result = std::make_unique<std::array<std::array<double, N>, N>>();

      // measure time taken
      timer.reset();

      blocked_matmul(A, B, *result, blk);
      elapsed_t = timer.elapsed_ms();

      std::cout << "[cache aware]" << N << " x " << N << " b: " << blk
                << (check_matrix_result(*result, C) ? " PASS" : " FAIL") << '\n';
      logger.log_timing("cache_aware", N, elapsed_t, blk, 0.0);
      // std::cout << "Elapsed time: " << elapsed_time << " ms" << '\n';

      result.reset();// free memory
    }
  }
}

}// namespace cache_aware
