#!/bin/bash

# 1. go to /combat-native-cpp
cd combat-native-cpp || { echo "Failed to cd into combat-native-cpp"; exit 1; }

# 2. create build and compile
mkdir -p build
cd build || { echo "Failed to cd into build directory"; exit 1; }
cmake .. || { echo "CMake failed"; exit 1; }
cmake --build . -j || { echo "Build failed"; exit 1; }
