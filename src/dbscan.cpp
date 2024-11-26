#include "dbscan.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <omp.h>
#include <stdexcept>

using namespace std;


// Region Query Function
std::vector<int> regionQuery(const std::vector<Point>& points, int pointIdx, double eps) {
    std::vector<std::vector<int>> threadNeighbors(omp_get_max_threads());

    #pragma omp parallel
    {
        int threadId = omp_get_thread_num();
        std::vector<int>& localNeighbors = threadNeighbors[threadId];

        #pragma omp for
        for (size_t i = 0; i < points.size(); i++) {
            if (euclideanDistance(points[pointIdx], points[i]) <= eps) {
                localNeighbors.push_back(i);
            }
        }
    }

    std::vector<int> neighbors;
    for (const auto& localNeighbors : threadNeighbors) {
        neighbors.insert(neighbors.end(), localNeighbors.begin(), localNeighbors.end());
    }
    return neighbors;
}

// Expand Cluster Function
void expandCluster(std::vector<Point>& points, int pointIdx, const std::vector<int>& neighbors,
                   int clusterId, double eps, int minPts) {
    points[pointIdx].cluster = clusterId;

    std::queue<int> neighborQueue;
    std::unordered_set<int> processed;

    for (int neighbor : neighbors) {
        neighborQueue.push(neighbor);
    }

    while (!neighborQueue.empty()) {
        int current = neighborQueue.front();
        neighborQueue.pop();

        if (processed.count(current)) continue; // Skip already processed neighbors
        processed.insert(current);

        if (points[current].cluster == -1) {
            points[current].cluster = clusterId; // Previously marked as noise
        }
        if (points[current].cluster == 0) { // Unvisited point
            points[current].cluster = clusterId;

            std::vector<int> currentNeighbors = regionQuery(points, current, eps);
            if (currentNeighbors.size() >= minPts) {
                for (int n : currentNeighbors) {
                    if (!processed.count(n)) {
                        neighborQueue.push(n);
                    }
                }
            }
        }
    }
}

// DBSCAN Algorithm
std::vector<Point> dbscan(std::vector<Point>& points, double eps, int minPts) {
    if (eps <= 0) {
        throw std::invalid_argument("DBSCAN: 'eps' must be greater than 0.");
    }
    if (minPts <= 0) {
        throw std::invalid_argument("DBSCAN: 'minPts' must be greater than 0.");
    }
    if (points.empty()) {
        throw std::invalid_argument("DBSCAN: Input points cannot be empty.");
    }

    int clusterId = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].cluster > 0) continue; // Skip already processed points

        std::vector<int> neighbors = regionQuery(points, i, eps);
        if (neighbors.size() < minPts) {
            points[i].cluster = -1; // Mark as noise
        } else {
            clusterId++;
            points[i].cluster = clusterId; // Assign initial cluster here
            expandCluster(points, i, neighbors, clusterId, eps, minPts);
        }
    }

    return points;
}
