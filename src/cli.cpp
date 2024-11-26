#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "dbscan.h"
#include "kmeans.h"
#include "point.h"

void printUsage() {
    std::cout << "Usage: clustering_toolbox <algorithm> [options]\n";
    std::cout << "\nAvailable algorithms:\n";
    std::cout << "  dbscan  : Run DBSCAN clustering\n";
    std::cout << "  kmeans  : Run K-Means clustering\n";
    std::cout << "\nOptions for DBSCAN:\n";
    std::cout << "  --eps <value>       : Maximum distance between two points to be considered neighbors\n";
    std::cout << "  --minPts <value>    : Minimum number of points required to form a dense region\n";
    std::cout << "\nOptions for K-Means:\n";
    std::cout << "  --k <value>         : Number of clusters\n";
    std::cout << "  --maxIterations <value> : Maximum iterations for convergence\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string algorithm = argv[1];
    std::vector<Point> points = {
        {1.0, 2.0}, {2.0, 3.0}, {3.0, 4.0}, {5.0, 7.0}, {8.0, 8.0}
    };

    try {
        if (algorithm == "dbscan") {
            double eps = 0.5;
            int minPts = 2;

            for (int i = 2; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--eps" && i + 1 < argc) {
                    eps = std::stod(argv[++i]);
                } else if (arg == "--minPts" && i + 1 < argc) {
                    minPts = std::stoi(argv[++i]);
                } else {
                    throw std::invalid_argument("Invalid argument for DBSCAN: " + arg);
                }
            }

            std::vector<Point> result = dbscan(points, eps, minPts);
            for (const auto& point : result) {
                point.print();
            }
        } else if (algorithm == "kmeans") {
            int k = 2;
            int maxIterations = 100;

            for (int i = 2; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--k" && i + 1 < argc) {
                    k = std::stoi(argv[++i]);
                } else if (arg == "--maxIterations" && i + 1 < argc) {
                    maxIterations = std::stoi(argv[++i]);
                } else {
                    throw std::invalid_argument("Invalid argument for K-Means: " + arg);
                }
            }

            std::vector<Point> result = kmeans(points, k, maxIterations);
            for (const auto& point : result) {
                point.print();
            }
        } else {
            throw std::invalid_argument("Invalid algorithm name: " + algorithm);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

