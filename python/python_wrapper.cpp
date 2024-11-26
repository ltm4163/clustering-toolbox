#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "dbscan.h"
#include "kmeans.h"

namespace py = pybind11;

// Helper to convert C++ Points to Python tuples
std::vector<std::tuple<std::vector<double>, int>> convert_to_python(const std::vector<Point>& points) {
    std::vector<std::tuple<std::vector<double>, int>> result;
    for (const auto& point : points) {
        result.emplace_back(point.coordinates, point.cluster);
    }
    return result;
}

// Python wrapper for DBSCAN
std::vector<std::tuple<std::vector<double>, int>> py_dbscan(
    const std::vector<std::vector<double>>& data, double eps, int minPts) {
    
    std::vector<Point> points;
    for (const auto& coords : data) {
        points.emplace_back(coords);
    }

    std::vector<Point> clustered_points = dbscan(points, eps, minPts);
    return convert_to_python(clustered_points);
}

// Python wrapper for K-Means
std::vector<std::tuple<std::vector<double>, int>> py_kmeans(
    const std::vector<std::vector<double>>& data, int k, int maxIterations) {
    
    std::vector<Point> points;
    for (const auto& coords : data) {
        points.emplace_back(coords);
    }

    std::vector<Point> clustered_points = kmeans(points, k, maxIterations);
    return convert_to_python(clustered_points);
}

// Pybind11 module definition
PYBIND11_MODULE(clusterbox, m) {
    m.doc() = "Clustering algorithms (DBSCAN and K-Means)";

    // DBSCAN function
    m.def("dbscan", &py_dbscan, "Run DBSCAN clustering",
          py::arg("data"), py::arg("eps"), py::arg("minPts"));

    // K-Means function
    m.def("kmeans", &py_kmeans, "Run K-Means clustering",
          py::arg("data"), py::arg("k"), py::arg("maxIterations"));
}

