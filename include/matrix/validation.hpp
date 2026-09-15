/*
 * Functions to validate matrix multiplication results.
 *
 */

#pragma once

#include <iostream>

template<std::size_t N>
bool check_matrix_result(const std::array<std::array<double, N>, N> &actual,
  const std::array<std::array<double, N>, N> &expected)
{
  constexpr double tolerance = 1e-1;

  for (std::size_t i = 0; i < actual.size(); ++i) {
    for (std::size_t j = 0; j < actual.at(i).size(); ++j) {
      const double difference = std::abs(actual.at(i).at(j) - expected.at(i).at(j));

      if (difference > tolerance) {
        std::cerr << "Mismatch at [" << i << "][" << j << "]\n"
                  << "Actual: " << actual.at(i).at(j) << "\n"
                  << "Expected: " << expected.at(i).at(j) << "\n"
                  << "Difference: " << difference << "\n";
        return false;
      }
    }
  }

  return true;
}
