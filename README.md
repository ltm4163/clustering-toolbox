Clustering Toolbox
This project implements clustering algorithms, including DBSCAN and K-Means, in C++ with Python bindings via pybind11. These algorithms are accessible in Python for further testing and visualization. The GPU implementation has been excluded from the final project due to unresolved issues.

Features
DBSCAN:
Density-based clustering algorithm.
Configurable parameters:
eps: Maximum distance between two points to be considered neighbors.
minPts: Minimum number of points required to form a dense region.
K-Means:
Centroid-based clustering algorithm.
Configurable parameters:
k: Number of clusters.
maxIterations: Maximum iterations to converge.
Project Structure
Directories
src/: Contains the core implementation files.
dbscan.cpp: DBSCAN implementation.
kmeans.cpp: K-Means implementation.
utils.cpp: Shared utility functions (e.g., distance calculations).
include/: Header files.
dbscan.h: Declaration for DBSCAN.
kmeans.h: Declaration for K-Means.
utils.h: Shared utilities.
point.h: Definition of the Point struct.
build/: Directory for compiled files.
test.py: Python test file for running and visualizing clustering algorithms.
Installation
Dependencies
Python 3.7 or higher
pybind11: Python bindings for C++ (Installation Guide)
CMake: Build system for the project
A C++ compiler supporting C++17
Build Instructions
Clone the repository:
bash
Copy code
git clone <repository_url>
cd clustering-toolbox
Build the project:
bash
Copy code
mkdir build && cd build
cmake -DPYTHON_EXECUTABLE=$(which python) ..
make
Ensure the clusterbox module is generated in the build/ directory:
bash
Copy code
build/clusterbox.cpython-<version>-x86_64-linux-gnu.so
Usage
Python Wrapper
The clustering algorithms can be accessed via the Python module clusterbox.

DBSCAN
python
Copy code
import clusterbox

# Input: List of points as tuples (x, y)
data = [(0.5, 1.0), (1.0, 1.5), (3.5, 4.0)]
eps = 0.5
minPts = 2

# Run DBSCAN
results = clusterbox.dbscan(data, eps, minPts)
# Output: List of points with cluster labels
print(results)  # [(0.5, 1.0, 0), (1.0, 1.5, 0), (3.5, 4.0, -1)]
K-Means
python
Copy code
import clusterbox

# Input: List of points as tuples (x, y)
data = [(0.5, 1.0), (1.0, 1.5), (3.5, 4.0)]
k = 2
maxIterations = 100

# Run K-Means
results = clusterbox.kmeans(data, k, maxIterations)
# Output: List of points with cluster labels
print(results)  # [(0.5, 1.0, 0), (1.0, 1.5, 0), (3.5, 4.0, 1)]
Testing
Run the provided Python test script to evaluate the clustering algorithms on synthetic datasets:

bash
Copy code
python3 test.py
This will:

Generate synthetic datasets (e.g., blobs, moons).
Run DBSCAN and K-Means on the datasets.
Save the results as images in the out/ directory.
Known Issues
The GPU implementation for DBSCAN and K-Means was scrapped due to performance and accuracy issues.
Future improvements may revisit GPU acceleration with better optimization techniques.
Future Work
Implement additional clustering algorithms.
Optimize current algorithms for larger datasets.
Reintroduce GPU support with focused debugging and testing.
