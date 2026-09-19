/*
 * Define different matrix sizes to test
 * NOTE: NOT USED
 */

// Four power-of-two sizes 128, 256, 512, 1024

#pragma once

#include <array>
#include <cstddef>
constexpr std::size_t MATRIX_SIZE_S_P2 = 128;
constexpr std::size_t MATRIX_SIZE_M_P2 = 256;
constexpr std::size_t MATRIX_SIZE_L_P2 = 512;
constexpr std::size_t MATRIX_SIZE_XL_P2 = 1024;

// Two non-power-of-two sizes 500, 750
constexpr std::size_t MATRIX_SIZE_S_NP2 = 500;
constexpr std::size_t MATRIX_SIZE_M_NP2 = 750;


// enum class MatrixSize {
//   S_P2 = MATRIX_SIZE_S_P2,
//   M_P2 = MATRIX_SIZE_M_P2,
//   L_P2 = MATRIX_SIZE_L_P2,
//   XL_P2 = MATRIX_SIZE_XL_P2,
//   M_NP2 = MATRIX_SIZE_M_NP2,
//   L_NP2 = MATRIX_SIZE_L_NP2
// };

// Design as static const vector to avoid reallocation and copying
// 1d array over 2d array to avoid memory fragmentation and improve cache locality
static const std::array<double, MATRIX_SIZE_S_P2> MATRIX_S_P2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_S_P2> MATRIX_S_P2_B = { 0.0 };
// Answer to crosscheck
static const std::array<double, MATRIX_SIZE_S_P2> MATRIX_S_P2_C = { 0.0 };

static const std::array<double, MATRIX_SIZE_M_P2> MATRIX_M_P2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_M_P2> MATRIX_M_P2_B = { 0.0 };
static const std::array<double, MATRIX_SIZE_M_P2> MATRIX_M_P2_C = { 0.0 };

static const std::array<double, MATRIX_SIZE_L_P2> MATRIX_L_P2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_L_P2> MATRIX_L_P2_B = { 0.0 };
static const std::array<double, MATRIX_SIZE_L_P2> MATRIX_L_P2_C = { 0.0 };

static const std::array<double, MATRIX_SIZE_XL_P2> MATRIX_XL_P2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_XL_P2> MATRIX_XL_P2_B = { 0.0 };
static const std::array<double, MATRIX_SIZE_XL_P2> MATRIX_XL_P2_C = { 0.0 };

static const std::array<double, MATRIX_SIZE_S_NP2> MATRIX_S_NP2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_S_NP2> MATRIX_S_NP2_B = { 0.0 };
static const std::array<double, MATRIX_SIZE_S_NP2> MATRIX_S_NP2_C = { 0.0 };

static const std::array<double, MATRIX_SIZE_M_NP2> MATRIX_M_NP2_A = { 0.0 };
static const std::array<double, MATRIX_SIZE_M_NP2> MATRIX_M_NP2_B = { 0.0 };
static const std::array<double, MATRIX_SIZE_M_NP2> MATRIX_M_NP2_C = { 0.0 };
