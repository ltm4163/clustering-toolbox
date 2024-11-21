#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "dbscan.h"
#include "dbscan_gpu.h"
#include "kmeans.h"
#include "kmeans_gpu.h"

namespace py = pybind11;

// Helper to convert C++ Points to Python tuples
std::vector<std::tuple<double, double, int>> convert_to_python(const std::vector<Point>& points) {
    std::vector<std::tuple<double, double, int>> result;
    for (const auto& point : points) {
        result.emplace_back(point.x, point.y, point.cluster);
    }
    return result;
}

// Python wrapper for CPU DBSCAN
std::vector<std::tuple<double, double, int>> py_dbscan(const std::vector<std::tuple<double, double>>& data, double eps, int minPts) {
    std::vector<Point> points;
    for (const auto& [x, y] : data) {
        points.push_back({x, y, 0});
    }

    std::vector<Point> clustered_points = dbscan(points, eps, minPts);
    return convert_to_python(clustered_points);
}

// Python wrapper for GPU DBSCAN
std::vector<std::tuple<double, double, int>> py_dbscan_gpu(const std::vector<std::tuple<double, double>>& data, double eps, int minPts) {
    std::vector<Point> points;
    for (const auto& [x, y] : data) {
        points.push_back({x, y, 0});
    }

    std::vector<Point> clustered_points = dbscan_gpu(points, eps, minPts);
    return convert_to_python(clustered_points);
}

// Python wrapper for CPU K-Means
std::vector<std::tuple<double, double, int>> py_kmeans(const std::vector<std::tuple<double, double>>& data, int k, int maxIterations) {
    std::vector<Point> points;
    for (const auto& [x, y] : data) {
        points.push_back({x, y, 0});
    }

    std::vector<Point> clustered_points = kmeans(points, k, maxIterations);
    return convert_to_python(clustered_points);
}

// Python wrapper for GPU K-Means
std::vector<std::tuple<double, double, int>> py_kmeans_gpu(const std::vector<std::tuple<double, double>>& data, int k, int maxIterations) {
    std::vector<Point> points;
    for (const auto& [x, y] : data) {
        points.push_back({x, y, 0});
    }

    std::vector<Point> clustered_points = kmeans_gpu(points, k, maxIterations);
    return convert_to_python(clustered_points);
}

// Pybind11 module definition
PYBIND11_MODULE(clusterbox, m) {
    m.doc() = "Clustering algorithms (DBSCAN and K-Means) with CPU and GPU support";

    // CPU implementations
    m.def("dbscan", &py_dbscan, "Run DBSCAN clustering (CPU)",
          py::arg("data"), py::arg("eps"), py::arg("minPts"));
    m.def("kmeans", &py_kmeans, "Run K-Means clustering (CPU)",
          py::arg("data"), py::arg("k"), py::arg("maxIterations"));

    // GPU implementations
    m.def("dbscan_gpu", &py_dbscan_gpu, "Run DBSCAN clustering (GPU)",
          py::arg("data"), py::arg("eps"), py::arg("minPts"));
    m.def("kmeans_gpu", &py_kmeans_gpu, "Run K-Means clustering (GPU)",
          py::arg("data"), py::arg("k"), py::arg("maxIterations"));
}

