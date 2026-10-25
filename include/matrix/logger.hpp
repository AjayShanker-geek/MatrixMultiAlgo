/*
 * Simple logger class to write timing results to a CSV file.
 *
 */

#pragma once

#include <fstream>
#include <stdexcept>
#include <string>

class Logger
{
public:
  explicit Logger(const std::string &filename) : file_(filename)
  {
    if (!file_) { throw std::runtime_error("Failed to open log file"); }

    // Write the header line to the CSV file
    file_ << "algorithm,matrix_size,time_ms,block_or_threshold,elapsed_padding_ms\n";
  }

  void log_timing(const std::string &algorithm,
    std::size_t matrix_size,
    double time_ms,
    int block_or_threshold,
    double elapsed_padding_t)
  {
    file_ << algorithm << ',' << matrix_size << ',' << time_ms << ',' << block_or_threshold << ',' << elapsed_padding_t
          << '\n';
  }

private:
  std::ofstream file_;
};
