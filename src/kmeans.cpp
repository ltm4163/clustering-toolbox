#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
#include <omp.h>
#include <iostream>
#include "kmeans.h"
#include "point.h"

// Function to calculate Euclidean distance between two points
double distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Function to initialize centroids using K-Means++
std::vector<Point> initializeCentroids(const std::vector<Point>& points, int k) {
    std::vector<Point> centroids;
    std::srand(std::time(0));
    centroids.push_back(points[std::rand() % points.size()]);

    // K-Means++ Initialization: Choose next centroids based on distance
    for (int i = 1; i < k; ++i) {
        double maxDist = 0;
        int newCentroidIdx = 0;

        // Find a new point that maximizes the minimum distance to the existing centroids
        for (int j = 0; j < points.size(); ++j) {
            double minDist = std::numeric_limits<double>::max();
            for (const auto& centroid : centroids) {
                double dist = distance(points[j], centroid);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
            if (minDist > maxDist) {
                maxDist = minDist;
                newCentroidIdx = j;
            }
        }

        centroids.push_back(points[newCentroidIdx]);
    }

    return centroids;
}

// K-Means Clustering Function
void kMeans(std::vector<Point>& points, int k, int maxIterations) {
    int n = points.size();
    std::vector<Point> centroids = initializeCentroids(points, k); // Initialize centroids
    std::vector<int> labels(n, 0);   // Cluster assignments

    for (int iter = 0; iter < maxIterations; ++iter) {
        bool hasConverged = true;

        // Step 1: Assign points to the nearest centroid
        #pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            double minDist = std::numeric_limits<double>::max();
            int bestCluster = 0;
            for (int j = 0; j < k; ++j) {
                double dist = distance(points[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }
            labels[i] = bestCluster;
        }

        // Step 2: Update centroids
        std::vector<Point> newCentroids(k, {0.0, 0.0});
        std::vector<int> count(k, 0);

        #pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            int cluster = labels[i];
            #pragma omp atomic
            newCentroids[cluster].x += points[i].x;
            #pragma omp atomic
            newCentroids[cluster].y += points[i].y;
            #pragma omp atomic
            count[cluster] += 1;
        }

        #pragma omp parallel for
        for (int j = 0; j < k; ++j) {
            if (count[j] > 0) {
                newCentroids[j].x /= count[j];
                newCentroids[j].y /= count[j];
            } else {
                newCentroids[j] = centroids[j]; // Keep old centroid if no points were assigned
            }
        }

        // Step 3: Check for convergence
        #pragma omp parallel for
        for (int j = 0; j < k; ++j) {
            if (distance(newCentroids[j], centroids[j]) > 1e-5) {
                hasConverged = false;
            }
        }

        centroids = newCentroids;

        if (hasConverged) {
            std::cout << "Converged after " << iter + 1 << " iterations.\n";
            break;
        }
    }

    // Output final cluster assignments
    std::cout << "Final centroids:\n";
    for (int j = 0; j < k; ++j) {
        std::cout << "Cluster " << j << ": (" << centroids[j].x << ", " << centroids[j].y << ")\n";
    }
}

