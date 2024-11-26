#!/bin/bash
# compile_python.sh

# Check architecture
ARCH=$(uname -m)

# Paths
SRC_DIR="../src"
WRAPPER="python_wrapper.cpp"
INCLUDE_DIR="../include"
OUTPUT_FILE="clusterbox.so"

# Compiler flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -O3 -shared -fPIC"
PYBIND11_FLAGS=$(python3 -m pybind11 --includes)

# OpenMP compatibility
if [ "$ARCH" == "arm64" ]; then
    echo "Compiling for macOS M1 (ARM)"
    CXXFLAGS="$CXXFLAGS -Xpreprocessor -fopenmp -lomp"
else
    echo "Compiling for macOS x86 or Linux"
    CXXFLAGS="$CXXFLAGS -fopenmp"
fi

# Compile
$CXX $CXXFLAGS $PYBIND11_FLAGS \
    -I$INCLUDE_DIR \
    $SRC_DIR/dbscan.cpp $SRC_DIR/kmeans.cpp $WRAPPER \
    -o $OUTPUT_FILE

echo "Python shared library compiled: $OUTPUT_FILE"

