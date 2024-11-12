#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
#include <omp.h>
#include <iostream>

struct Point {
    double x, y;
};

// Function to calculate Euclidean distance between two points
double distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Function to randomly initialize centroids
std::vector<Point> initializeCentroids(const std::vector<Point>& points, int k) {
    std::vector<Point> centroids;
    std::srand(std::time(0));
    for (int i = 0; i < k; ++i) {
        int idx = std::rand() % points.size();
        centroids.push_back(points[idx]);
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
            if (distance(newCentroids[j], centroids[j]) > 1e-4) {
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


