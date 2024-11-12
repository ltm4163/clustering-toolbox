#include <iostream>
#include <vector>
#include "point.h"
#include "kmeans.h"
#include "dbscan.h"

int main() {
    // Sample data points
    std::vector<Point> points = {
        {1.0, 2.0}, {1.5, 1.8}, {5.0, 8.0}, {8.0, 8.0},
        {1.0, 0.6}, {9.0, 11.0}, {8.0, 2.0}, {10.0, 2.0},
        {9.0, 3.0}
    };

    // Parameters for K-Means
    int k = 3;
    int maxIterations = 100;
    kMeans(points, k, maxIterations);

    // Parameters for DBSCAN
    double epsilon = 2.5;
    int minPoints = 2;
    std::vector<int> dbscanLabels = dbscan(points, epsilon, minPoints);

    // Print DBSCAN results
    std::cout << "DBSCAN Labels:\n";
    for (size_t i = 0; i < dbscanLabels.size(); ++i) {
        std::cout << "Point " << i << ": Cluster " << dbscanLabels[i] << "\n";
    }

    return 0;
}

