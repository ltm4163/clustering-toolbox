@echo off
REM compile_python.bat

REM Paths
set SRC_DIR=../src
set WRAPPER=python_wrapper.cpp
set INCLUDE_DIR=../include
set OUTPUT_FILE=clusterbox.pyd

REM Compiler flags
set CXX=g++
set CXXFLAGS=-std=c++17 -Wall -O3 -shared -fPIC
for /f %%i in ('python -m pybind11 --includes') do set PYBIND11_FLAGS=%%i

REM OpenMP support
set CXXFLAGS=%CXXFLAGS% -fopenmp

REM Compile
%CXX% %CXXFLAGS% %PYBIND11_FLAGS% -I%INCLUDE_DIR% %SRC_DIR%/dbscan.cpp %SRC_DIR%/kmeans.cpp %WRAPPER% -o %OUTPUT_FILE%

echo Python shared library compiled: %OUTPUT_FILE%

