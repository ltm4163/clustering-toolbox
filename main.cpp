#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "point.h"
#include "kmeans.h"
#include "dbscan.h"

std::vector<Point> readPointsFromFile(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    double x, y;
    while (file >> x >> y) {
        points.push_back({x, y});
    }
    return points;
}

void saveClusteringResults(const std::vector<Point>& points, const std::vector<int>& labels, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < points.size(); ++i) {
        file << points[i].x << " " << points[i].y << " " << labels[i] << "\n";
    }
}

int main() {
    std::vector<Point> points = readPointsFromFile("points.txt");
    if (points.empty()) {
        std::cerr << "No points found in file." << std::endl;
        return 1;
    }

    int k = 3;
    int maxIterations = 100;
    double epsilon = 2.5;
    int minPoints = 5;

    // K-Means Clustering and Timing
    std::vector<int> kmeansLabels(points.size(), 0);
    auto start_kmeans = std::chrono::high_resolution_clock::now();
    kMeans(points, k, maxIterations);
    auto end_kmeans = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> kmeans_duration = end_kmeans - start_kmeans;
    std::cout << "K-Means execution time: " << kmeans_duration.count() << " seconds\n";

    // Save K-Means Results
    saveClusteringResults(points, kmeansLabels, "kmeans_results.txt");

    // DBSCAN Clustering and Timing
    auto start_dbscan = std::chrono::high_resolution_clock::now();
    std::vector<int> dbscanLabels = dbscan(points, epsilon, minPoints);
    auto end_dbscan = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dbscan_duration = end_dbscan - start_dbscan;
    std::cout << "DBSCAN execution time: " << dbscan_duration.count() << " seconds\n";

    // Save DBSCAN Results
    saveClusteringResults(points, dbscanLabels, "dbscan_results.txt");

    return 0;
}

