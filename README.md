# Algorithms & Performance Optimization

The project focuses on implementing three matrix multiplication algorithms: naive, cache-aware, and cache-oblivious. The benchmark
will generate random matrices of size `N x N` for `N = 128, 256, 512, 1024, 500, 750`, `b = 8, 16, 32, 64, 128` for cache-aware algorithms and `t = 8, 16, 32, 64, 128` for cache-oblivious algorithms, and measure the execution time for each algorithm. The results will be stored in CSV files for analysis. The task written in C++20 and build using CMake

## Project Setup

Project structure reference from [Link](https://github.com/cpp-best-practices/cmake_template)

- Using CMake for build configuration for creating three binary for each sub-tasks
- `logs` directory is used to store the logs generated during the execution of the programs.
- `plots` uses python's matplotlib library to visualize the performance of the algorithms.
- `figures` directory is used to store the generated plots and figures.
- `build` directory is used to store the compiled binaries and other build artifacts.
- `src` directory contains the source code for the project, organized into subdirectories for each sub-task.
- `include` directory contains the header files for the project, organized into subdirectories for each sub-task.

## Building the Project

```bash
mkdir build
cd build

cmake ..
cmake --build build

# Running the binaries for each sub-task

./build/naive_matmul
./build/cache_aware_matmul
./build/cache_oblivious_matmul
# OR
./run.sh
```

## Generate Plots

```bash
# Create a virtual environment for Python dependencies
python3 -m venv venv
source venv/bin/activate

pip3 install -r requirements.txt

# Run the plotting script to generate performance plots
python3 plots/plot.py
```
