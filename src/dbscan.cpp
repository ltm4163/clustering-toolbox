#include "dbscan.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <omp.h>

using namespace std;

// Find all neighbors of a point within the given radius (eps)
std::vector<int> regionQuery(const std::vector<Point>& points, int pointIdx, double eps) {
    std::vector<int> neighbors;
    #pragma omp parallel for
    for (size_t i = 0; i < points.size(); i++) {
        if (euclideanDistance(points[pointIdx], points[i]) <= eps) {
            #pragma omp critical
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// Expand the cluster starting from the given point
void expandCluster(std::vector<Point>& points, int pointIdx, const std::vector<int>& neighbors,
                   int clusterId, double eps, int minPts) {
    points[pointIdx].cluster = clusterId;

    std::queue<int> neighborQueue;
    for (int neighbor : neighbors) {
        neighborQueue.push(neighbor);
    }

    while (!neighborQueue.empty()) {
        int current = neighborQueue.front();
        neighborQueue.pop();

        if (points[current].cluster == -1) {
            points[current].cluster = clusterId; // Previously marked as noise
        }
        if (points[current].cluster == 0) { // Unvisited point
            points[current].cluster = clusterId;

            std::vector<int> currentNeighbors = regionQuery(points, current, eps);
            if (currentNeighbors.size() >= minPts) {
                for (int n : currentNeighbors) {
                    neighborQueue.push(n);
                }
            }
        }
    }
}

// DBSCAN algorithm implementation
std::vector<Point> dbscan(std::vector<Point>& points, double eps, int minPts) {
    int clusterId = 0;
    #pragma omp parallel for
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].cluster != 0) continue;

        std::vector<int> neighbors = regionQuery(points, i, eps);
        if (neighbors.size() < minPts) {
            points[i].cluster = -1; // Mark as noise
        } else {
            #pragma omp critical
            {
            clusterId++;
            }
            expandCluster(points, i, neighbors, clusterId, eps, minPts);
        }
    }

    return points;
}

