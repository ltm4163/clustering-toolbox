cmake_minimum_required(VERSION 3.10)
project(ClusteringToolbox)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenMP REQUIRED)

include_directories(include)

add_executable(clustering-toolbox main.cpp src/kmeans.cpp src/dbscan.cpp)
target_link_libraries(clustering-toolbox OpenMP::OpenMP_CXX)
