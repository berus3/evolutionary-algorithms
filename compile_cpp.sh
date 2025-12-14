#!/bin/bash

# 1. go to /combat-native-cpp
cd combat-native-cpp || { echo "Failed to cd into combat-native-cpp"; exit 1; }

# 2. create build and compile
mkdir -p build
cd build || { echo "Failed to cd into build directory"; exit 1; }
cmake .. || { echo "CMake failed"; exit 1; }
cmake --build . -j || { echo "Build failed"; exit 1; }

# 3. go to /combat-native-cpp
cd ..

# 4. copy libcombat.so to evolver-java/native
cp build/lib/libcombat.so ../evolver-java/native/libcombat.so || { echo "Failed to copy libcombat.so"; exit 1; }