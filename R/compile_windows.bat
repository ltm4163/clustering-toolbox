@echo off
REM compile_r.bat

REM Paths
set SRC_DIR=src
set WRAPPER=R/rcpp_wrapper.cpp
set INCLUDE_DIR=include
set OUTPUT_FILE=R/clusterbox.dll

REM Compiler flags
set CXX=g++
set CXXFLAGS=-std=c++17 -Wall -O3 -shared -fPIC
for /f %%i in ('Rscript -e "cat(Rcpp:::CxxFlags())"') do set RCPP_FLAGS=%%i
for /f %%i in ('Rscript -e "cat(Rcpp:::LdFlags())"') do set R_LDFLAGS=%%i

REM OpenMP support
set CXXFLAGS=%CXXFLAGS% -fopenmp

REM Compile
%CXX% %CXXFLAGS% %RCPP_FLAGS% -I%INCLUDE_DIR% %SRC_DIR%/dbscan.cpp %SRC_DIR%/kmeans.cpp %WRAPPER% %R_LDFLAGS% -o %OUTPUT_FILE%

echo R shared library compiled: %OUTPUT_FILE%

