#!/bin/bash
# compile_r.sh

# Paths
SRC_DIR="../src"
WRAPPER="rcpp_wrapper.cpp"
INCLUDE_DIR="../include"
OUTPUT_FILE="clusterbox.so"

# Compiler flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -O3 -shared -fPIC -I/usr/share/R/include"
RCPP_FLAGS=$(Rscript -e "Rcpp:::CxxFlags()")
R_LDFLAGS=$(Rscript -e "Rcpp:::LdFlags()")

# OpenMP compatibility
if [ "$(uname -m)" == "arm64" ]; then
    echo "Compiling for macOS M1 (ARM)"
    CXXFLAGS="$CXXFLAGS -Xpreprocessor -fopenmp -lomp"
else
    echo "Compiling for macOS x86 or Linux"
    CXXFLAGS="$CXXFLAGS -fopenmp"
fi

# Compile
$CXX $CXXFLAGS $RCPP_FLAGS \
    -I$INCLUDE_DIR \
    $SRC_DIR/dbscan.cpp $SRC_DIR/kmeans.cpp $WRAPPER $R_LDFLAGS \
    -o $OUTPUT_FILE

echo "R shared library compiled: $OUTPUT_FILE"

