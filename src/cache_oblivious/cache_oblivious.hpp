#pragma once

#include <iostream>

#include <cassert>
#include <cstddef>
#include <vector>

#include "logger.hpp"
#include "param.hpp"
#include "timer.hpp"
#include "validation.hpp"


namespace cache_oblivious {
static const std::vector<int> t_arr = { 8, 16, 32, 64, 128 };

// Compile time check if a number is a power of two using constexpr
constexpr bool is_power_of_two(std::size_t value)
{

  bool isNotZero_ = (value != 0);
  // Check if value is a power of two using bitwise operation
  // Power of two if only one bit is set in the binary representation
  bool isPowerOfTwo_ = ((value & (value - 1)) == 0);

  return isNotZero_ && isPowerOfTwo_;
}


constexpr std::size_t next_power_of_two(std::size_t value)
{
  std::size_t result_ = 1;

  while (result_ < value) { result_ *= 2; }

  return result_;
}

template<std::size_t N>
void naive_submatrix(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  std::array<std::array<double, N>, N> &C,
  std::size_t ar,
  std::size_t ac,
  std::size_t br,
  std::size_t bc,
  std::size_t cr,
  std::size_t cc,
  std::size_t size)
{
  for (std::size_t i = 0; i < size; ++i) {
    for (std::size_t j = 0; j < size; ++j) {
      for (std::size_t k = 0; k < size; ++k) {
        C.at(cr + i).at(cc + j) += A.at(ar + i).at(ac + k) * B.at(br + k).at(bc + j);
      }
    }
  }
}

template<std::size_t N>
void cache_oblivious_recursive(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  std::array<std::array<double, N>, N> &C,
  std::size_t ar,// row index of A
  std::size_t ac,// column index of A
  std::size_t br,
  std::size_t bc,
  std::size_t cr,
  std::size_t cc,
  std::size_t size,
  std::size_t threshold)
{
  if (size <= threshold) {
    naive_submatrix(A, B, C, ar, ac, br, bc, cr, cc, size);

    return;
  }

  const std::size_t first_size = size / 2;
  const std::size_t second_size = size - first_size;

  // C11 = A11 * B11 + A12 * B21
  cache_oblivious_recursive(A, B, C, ar, ac, br, bc, cr, cc, first_size, threshold);

  cache_oblivious_recursive(A, B, C, ar, ac + first_size, br + first_size, bc, cr, cc, first_size, threshold);

  // C12 = A11 * B12 + A12 * B22
  cache_oblivious_recursive(A, B, C, ar, ac, br, bc + first_size, cr, cc + first_size, first_size, threshold);

  cache_oblivious_recursive(
    A, B, C, ar, ac + first_size, br + first_size, bc + first_size, cr, cc + first_size, first_size, threshold);

  // C21 = A21 * B11 + A22 * B21
  cache_oblivious_recursive(A, B, C, ar + first_size, ac, br, bc, cr + first_size, cc, second_size, threshold);

  cache_oblivious_recursive(
    A, B, C, ar + first_size, ac + first_size, br + first_size, bc, cr + first_size, cc, second_size, threshold);

  // C22 = A21 * B12 + A22 * B22
  cache_oblivious_recursive(
    A, B, C, ar + first_size, ac, br, bc + first_size, cr + first_size, cc + first_size, second_size, threshold);

  cache_oblivious_recursive(A,
    B,
    C,
    ar + first_size,
    ac + first_size,
    br + first_size,
    bc + first_size,
    cr + first_size,
    cc + first_size,
    second_size,
    threshold);
}

template<std::size_t N>
void cache_oblivious_matmul(const std::array<std::array<double, N>, N> &A,
  const std::array<std::array<double, N>, N> &B,
  std::array<std::array<double, N>, N> &C,
  std::size_t threshold)
{
  std::size_t ar = 0, ac = 0, br = 0, bc = 0, cr = 0, cc = 0;
  cache_oblivious_recursive(A, B, C, ar, ac, br, bc, cr, cc, N, threshold);
}

template<std::size_t N> auto pad_matrix(const std::array<std::array<double, N>, N> &input)
{
  constexpr std::size_t P = next_power_of_two(N);

  // using PaddedMatrix = std::array<std::array<double, P>, P>;

  auto padded = std::make_unique<std::array<std::array<double, P>, P>>();

  assert(N <= P && "Original matrix size must be less than or equal to padded size.");

  // Copy the original matrix into the top-left corner of the padded matrix.
  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) { padded->at(i).at(j) = input.at(i).at(j); }
  }

  // Fill the remaining elements with zeros.
  for (std::size_t i = N; i < P; ++i) {
    for (std::size_t j = 0; j < P; ++j) { padded->at(i).at(j) = 0.0; }
  }

  return padded;
}

template<std::size_t N, std::size_t PaddedN>
void unpad_matrix(const std::array<std::array<double, PaddedN>, PaddedN> &padded,
  std::array<std::array<double, N>, N> &result)
{

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) { result.at(i).at(j) = padded.at(i).at(j); }
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
  double elapsed_padding_t{ 0.0 };

  for (int t : t_arr) {
    for (std::size_t i = 0; i < LOOP_EXPERIMENTAL; ++i) {
      // create a unique_ptr to hold the result matrix
      auto result = std::make_unique<std::array<std::array<double, N>, N>>();

      if (is_power_of_two(N)) {
        // measure time taken
        timer.reset();

        cache_oblivious_matmul(A, B, *result, t);
        elapsed_t = timer.elapsed_ms();


      } else {
        // Pad the matrices to the next power of two
        timer.reset();
        auto padded_A = pad_matrix(A);
        auto padded_B = pad_matrix(B);
        auto padded_result =
          std::make_unique<std::array<std::array<double, next_power_of_two(N)>, next_power_of_two(N)>>();
        elapsed_padding_t = timer.elapsed_ms();

        // measure time taken
        timer.reset();
        cache_oblivious_matmul(*padded_A, *padded_B, *padded_result, t);
        elapsed_t = timer.elapsed_ms();

        unpad_matrix<N, next_power_of_two(N)>(*padded_result, *result);
      }
      std::cout << "[cache oblivious]" << N << " x " << N << " t: " << t
                << (check_matrix_result(*result, C) ? " PASS" : " FAIL") << '\n';
      logger.log_timing("cache_oblivious", N, elapsed_t, t, elapsed_padding_t);

      result.reset();// free memory
    }
  }
}


}// namespace cache_oblivious
