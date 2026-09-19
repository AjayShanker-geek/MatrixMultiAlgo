/**
 * @file main.cpp
 * @brief This is the main file runs perform matrix multiplication using
 * cache-oblivious techniques.
 * @author Ajay Shanker
 *
 */

#include <iostream>

#include "cache_oblivious.hpp"
#include "logger.hpp"
#include "param.hpp"
#include "timer.hpp"
#include "validation.hpp"

#include "matrices_1024.hpp"
#include "matrices_128.hpp"
#include "matrices_256.hpp"
#include "matrices_500.hpp"
#include "matrices_512.hpp"
#include "matrices_750.hpp"

using namespace cache_oblivious;

int main()
{
  std::cout << "Running cache_oblivious" << '\n';

  timer::Timer timer;


  Logger logger("./logs/cache_oblivious_timings.csv");

  {
    using namespace matrices_128;

    run_benchmark(A, B, C, timer, logger);
  }

  {
    using namespace matrices_256;

    run_benchmark(A, B, C, timer, logger);
  }

  {
    using namespace matrices_512;

    run_benchmark(A, B, C, timer, logger);
  }

  {
    using namespace matrices_1024;

    run_benchmark(A, B, C, timer, logger);
  }

  {
    using namespace matrices_500;

    run_benchmark(A, B, C, timer, logger);
  }

  {
    using namespace matrices_750;

    run_benchmark(A, B, C, timer, logger);
  }
  return 0;
}
