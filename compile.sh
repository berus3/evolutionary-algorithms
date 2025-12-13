#!/bin/bash

# 1. go to /combat-native-cpp
cd combat-native-cpp || { echo "Failed to cd into combat-native-cpp"; exit 1; }

# 2. create build and compile
mkdir -p build
cd build || { echo "Failed to cd into build directory"; exit 1; }
cmake .. || { echo "CMake failed"; exit 1; }
cmake --build . -j || { echo "Build failed"; exit 1; }

# 3. go to /combat-native-cpp
#cd ..

# 4. copy libcombat.so to evolver-java/native
#cp build/lib/libcombat.so ../evolver-java/native/libcombat.so || { echo "Failed to copy libcombat.so"; exit 1; }

# 5. go to /evolver-java and run maven package and build-classpath
#cd ../evolver-java || { echo "Failed to cd into evolver-java"; exit 1; }
#mvn -q -DskipTests package || { echo "Maven package failed"; exit 1; }
#mvn -q dependency:build-classpath \
#  -DincludeScope=runtime \
#  -Dmdep.outputFile=cp.txt || { echo "Maven dependency classpath failed"; exit 1; }

echo "Build and Maven tasks completed successfully!"

