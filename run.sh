#!/bin/bash

cd evolver-java || { echo "Failed to cd into evolver-java"; exit 1; }
java --enable-native-access=ALL-UNNAMED \
  -Devol.debugEval=true \
  -Djava.library.path="$PWD/native" \
  -cp "target/classes:$(cat cp.txt)" \
  org.evol.MainEA || { echo "Java application failed to run"; exit 1; }
