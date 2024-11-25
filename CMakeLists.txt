cmake_minimum_required(VERSION 3.16)

project(ClusteringToolbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(/home/stu11/s12/ltm4163/R/x86_64-pc-linux-gnu-library/3.6/Rcpp/include)
include_directories(/usr/share/R/include)  

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)
find_package(OpenMP REQUIRED)

# Python module
pybind11_add_module(clusterbox
    src/python_wrapper.cpp
    src/dbscan.cpp
    src/kmeans.cpp
)

# R module
add_library(rclusterbox MODULE
    src/dbscan.cpp
    src/kmeans.cpp
    src/rcpp_wrapper.cpp
)

target_link_libraries(clusterbox PRIVATE Python3::Python)
target_link_libraries(rclusterbox PRIVATE ${R_LIBRARIES})

include_directories(${R_INCLUDE_DIRS} include)

set_target_properties(clusterbox PROPERTIES PREFIX "")
set_target_properties(rclusterbox PROPERTIES PREFIX "")

