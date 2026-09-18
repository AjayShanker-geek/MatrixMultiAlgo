#! /bin/sh

# Ensure latest binaries are built
cmake --build build

# Run the binaries
# Supercore from 0-5
./build/naive_matmul
sleep 2
./build/cache_aware_matmul
sleep 2
./build/cache_oblivious_matmul
