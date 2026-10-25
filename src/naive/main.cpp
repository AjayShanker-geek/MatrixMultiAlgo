/**
 * @file main.cpp
 * @brief This is the main file runs perform matrix.
 * @author Ajay Shanker
 *
 */

#include <cstddef>
#include <iostream>

#include "logger.hpp"
#include "naive.hpp"
#include "timer.hpp"

#include "matrices_1024.hpp"
#include "matrices_128.hpp"
#include "matrices_256.hpp"
#include "matrices_500.hpp"
#include "matrices_512.hpp"
#include "matrices_750.hpp"

using namespace naive;

int main()
{
  std::cout << "Running naive" << '\n';

  timer::Timer timer;

  Logger logger("./logs/naive_timings.csv");


  {
    using namespace matrices_128;

    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_256;


    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_512;


    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_512;

    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_1024;

    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_500;

    run_benchmark<N>(A, B, C, timer, logger);
  }

  {
    using namespace matrices_750;

    run_benchmark<N>(A, B, C, timer, logger);
  }

  return 0;
}
