#include "dbscan.h"
#include <cmath>
#include <vector>
#include <limits>
#include <omp.h>

std::vector<int> dbscan(const std::vector<Point>& points, double epsilon, int minPoints) {
    int n = points.size();
    std::vector<int> labels(n, -1); // -1 indicates noise
    int clusterID = 0;

    auto distance = [](const Point& a, const Point& b) {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    };

    auto regionQuery = [&](int idx) {
        std::vector<int> neighbors;
        for (int i = 0; i < n; ++i) {
            if (distance(points[idx], points[i]) <= epsilon) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    };

    auto expandCluster = [&](int idx, std::vector<int>& neighbors) {
        labels[idx] = clusterID;
        for (size_t i = 0; i < neighbors.size(); ++i) {
            int neighborIdx = neighbors[i];
            if (labels[neighborIdx] == -1) {
                labels[neighborIdx] = clusterID;
            }
            if (labels[neighborIdx] == 0) {
                labels[neighborIdx] = clusterID;
                std::vector<int> neighborNeighbors = regionQuery(neighborIdx);
                if (neighborNeighbors.size() >= minPoints) {
                    neighbors.insert(neighbors.end(), neighborNeighbors.begin(), neighborNeighbors.end());
                }
            }
        }
    };

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        if (labels[i] != -1) continue; // Skip if already labeled
        std::vector<int> neighbors = regionQuery(i);
        if (neighbors.size() < minPoints) {
            labels[i] = -1; // Mark as noise
        } else {
            clusterID++;
            expandCluster(i, neighbors);
        }
    }

    return labels;
}
