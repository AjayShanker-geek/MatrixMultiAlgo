from pathlib import Path

import numpy as np


SIZES = [128, 256, 512, 1024, 500, 750]
MIN_VALUE = -100.0
MAX_VALUE = 100.0
SEED = 42
OUTPUT_DIR = Path("../include/matrix/")
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)


def format_cpp_2d_array(matrix):
    rows = []

    for row in matrix:
        values = ", ".join(f"{value:.3f}" for value in row)
        rows.append(f"        {{{values}}}")

    # Extra outer braces are required for nested std::array.
    return "{{\n" + ",\n".join(rows) + "\n    }}"


def generate_header(size, rng):
    matrix_a = rng.uniform(MIN_VALUE, MAX_VALUE, size=(size, size)).round(3)

    matrix_b = rng.uniform(MIN_VALUE, MAX_VALUE, size=(size, size)).round(3)

    matrix_c = (matrix_a @ matrix_b).round(3)

    namespace = f"matrices_{size}"

    return f"""#pragma once

#include <array>
#include <cstddef>

namespace {namespace} {{

inline constexpr std::size_t N = {size};

inline const std::array<std::array<double, N>, N> A =
{format_cpp_2d_array(matrix_a)};

inline const std::array<std::array<double, N>, N> B =
{format_cpp_2d_array(matrix_b)};

inline const std::array<std::array<double, N>, N> C =
{format_cpp_2d_array(matrix_c)};

}} // namespace {namespace}
"""


rng = np.random.default_rng(SEED)

for size in SIZES:
    output_file = OUTPUT_DIR / f"matrices_{size}.hpp"

    header_code = generate_header(size, rng)

    output_file.write_text(header_code, encoding="utf-8")

    print(f"Generated: {output_file}")
