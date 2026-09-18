/*
 * Minimal wall-clock timer to benchmark code execution time in milliseconds.
 *
 */

#pragma once

#include <cassert>
#include <chrono>

// Recommeded for benchmark as steady_clock is monotonic and not affected by system clock changes
using Clock = std::chrono::steady_clock;

namespace timer {

class Timer
{

public:
  // Constructor starts the timer
  Timer() : start_(Clock::now()) {}

  // Reset the timer to the current time
  void reset() { start_ = Clock::now(); }

  // Return the elapsed time in milliseconds
  double elapsed_ms() const
  {
    const auto elapsed = Clock::now() - start_;
    const auto elapsed_ms = std::chrono::duration<double, std::milli>(elapsed).count();

    assert(elapsed_ms >= 0.0 && "Elapsed time should be non-negative");
    return elapsed_ms;
  }


private:
  // Ensure that start is not modified by the user
  std::chrono::high_resolution_clock::time_point start_;
};

}// namespace timer
