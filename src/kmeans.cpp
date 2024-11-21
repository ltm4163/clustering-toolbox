#include "kmeans.h"
#include "utils.h"
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <stdexcept> 

using namespace std;


// Initialize random centroids
std::vector<Point> initializeCentroids(const std::vector<Point>& points, int k) {
    std::vector<Point> centroids;
    std::srand(std::time(0));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(points[std::rand() % points.size()]);
    }
    return centroids;
}

// Assign points to the nearest centroid
std::vector<int> assignPointsToClusters(const std::vector<Point>& points, const std::vector<Point>& centroids) {
    std::vector<int> clusters(points.size());
    #pragma omp parallel for
    for (size_t i = 0; i < points.size(); ++i) {
        double minDistance = std::numeric_limits<double>::max();
        for (size_t j = 0; j < centroids.size(); ++j) {
            double distance = euclideanDistance(points[i], centroids[j]);
            if (distance < minDistance) {
                minDistance = distance;
                clusters[i] = j;
            }
        }
    }
    return clusters;
}

// Update centroids based on assigned clusters
std::vector<Point> updateCentroids(const std::vector<Point>& points, const std::vector<int>& clusters, int k) {
    std::vector<Point> centroids(k, {0, 0, 0});
    std::vector<int> counts(k, 0);
    #pragma omp parallel for
    for (size_t i = 0; i < points.size(); ++i) {
        #pragma omp critical 
        {
            centroids[clusters[i]].x += points[i].x;
            centroids[clusters[i]].y += points[i].y;
            counts[clusters[i]]++;
        }
    }
    
    #pragma omp parallel for
    for (int i = 0; i < k; ++i) {
        if (counts[i] > 0) {
            centroids[i].x /= counts[i];
            centroids[i].y /= counts[i];
        }
    }

    return centroids;
}

// K-Means algorithm implementation
std::vector<Point> kmeans(std::vector<Point>& points, int k, int maxIterations) {
    if (k <= 0) {
        throw std::invalid_argument("K-Means: 'k' must be greater than 0.");
    }
    if (k > points.size()) {
        throw std::invalid_argument("K-Means: 'k' must not exceed the number of points.");
    }
    if (maxIterations <= 0) {
        throw std::invalid_argument("K-Means: 'maxIterations' must be greater than 0.");
    }
    if (points.empty()) {
        throw std::invalid_argument("K-Means: Input points cannot be empty.");
    }

    // Check for invalid points
    for (const auto& point : points) {
        if (std::isnan(point.x) || std::isnan(point.y) || std::isinf(point.x) || std::isinf(point.y)) {
            throw std::invalid_argument("K-Means: Input points contain invalid coordinates.");
        }
    }

    // Initialize centroids randomly
    std::vector<Point> centroids = initializeCentroids(points, k);
    std::vector<int> clusters(points.size());

    for (int iter = 0; iter < maxIterations; ++iter) {
        // Assign points to nearest centroid
        clusters = assignPointsToClusters(points, centroids);

        // Update centroids
        std::vector<Point> newCentroids = updateCentroids(points, clusters, k);

        // Check for convergence
        if (newCentroids == centroids) {
            break;
        }

        centroids = newCentroids;
    }

    // Assign final clusters to points
    for (size_t i = 0; i < points.size(); ++i) {
        points[i].cluster = clusters[i];
    }

    return points;
}

