project(ClusteringToolbox LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(include)

pybind11_add_module(clusterbox
    python_wrapper.cpp
    src/dbscan.cpp
    src/kmeans.cpp
)

target_link_libraries(clusterbox PUBLIC OpenMP::OpenMP_CXX)

