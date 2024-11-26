cmake_minimum_required(VERSION 3.10)

project(ClusteringToolbox)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
    src/dbscan.cpp
    src/kmeans.cpp
    src/cli.cpp  # Assuming the CLI file is named cli.cpp
)

# Include directories
include_directories(include)

# Create executable
add_executable(clustering_toolbox ${SOURCES})

# Find OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(clustering_toolbox PUBLIC OpenMP::OpenMP_CXX)
endif()

# Set installation rules
install(TARGETS clustering_toolbox DESTINATION bin)

