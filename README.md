# **Clustering Toolbox**

This project implements clustering algorithms, including **DBSCAN** and **K-Means**, in C++ with Python bindings via `pybind11`. These algorithms are accessible in Python for further testing and visualization. The GPU implementation has been excluded from the final project due to unresolved issues.

---

## **Features**
- **DBSCAN**:
  - Density-based clustering algorithm.
  - Configurable parameters:
    - `eps`: Maximum distance between two points to be considered neighbors.
    - `minPts`: Minimum number of points required to form a dense region.
- **K-Means**:
  - Centroid-based clustering algorithm.
  - Configurable parameters:
    - `k`: Number of clusters.
    - `maxIterations`: Maximum iterations to converge.

---

## **Project Structure**

### **Directories**
- `src/`: Contains the core implementation files.
  - `dbscan.cpp`: DBSCAN implementation.
  - `kmeans.cpp`: K-Means implementation.
  - `utils.cpp`: Shared utility functions (e.g., distance calculations).
- `include/`: Header files.
  - `dbscan.h`: Declaration for DBSCAN.
  - `kmeans.h`: Declaration for K-Means.
  - `utils.h`: Shared utilities.
  - `point.h`: Definition of the `Point` struct.
- `build/`: Directory for compiled files.
- `test.py`: Python test file for running and visualizing clustering algorithms.

---

## **Installation**

### **Dependencies**
- Python 3.7 or higher
- `pybind11`: Python bindings for C++ ([Installation Guide](https://pybind11.readthedocs.io/en/stable/compiling.html))
- `CMake`: Build system for the project
- A C++ compiler supporting C++17

### **Build Instructions**
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd clustering-toolbox
   ```
2. Build the project:
    ```bash
    chmod +x build.sh
    ./build.sh
    ```
## **Usage**

### **Python Wrapper**
The clustering algorithms can be accessed via the Python module clusterbox


