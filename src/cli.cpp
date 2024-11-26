#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include "dbscan.h"
#include "kmeans.h"
#include "pca.h"
#include "point.h"

void printUsage() {
    std::cout << "Usage: clustering_toolbox <algorithm> [options] --input <input_file> --output <output_file>\n";
    std::cout << "\nAvailable algorithms:\n";
    std::cout << "  dbscan  : Run DBSCAN clustering\n";
    std::cout << "  kmeans  : Run K-Means clustering\n";
    std::cout << "  pca     : Reduce dimensionality using PCA\n";
    std::cout << "\nOptions for DBSCAN:\n";
    std::cout << "  --eps <value>       : Maximum distance between two points to be considered neighbors\n";
    std::cout << "  --minPts <value>    : Minimum number of points required to form a dense region\n";
    std::cout << "\nOptions for K-Means:\n";
    std::cout << "  --k <value>         : Number of clusters\n";
    std::cout << "  --maxIterations <value> : Maximum iterations for convergence\n";
    std::cout << "\nOptions for PCA:\n";
    std::cout << "  --dimensions <value> : Number of dimensions to reduce to (required for PCA)\n";
    std::cout << "\nRequired Arguments:\n";
    std::cout << "  --input <input_file>   : Input file containing points\n";
    std::cout << "  --output <output_file> : Output file to write results\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string algorithm = argv[1];
    std::string inputFile;
    std::string outputFile;
    double eps = 0.5;
    int minPts = 2;
    int k = 2;
    int maxIterations = 100;
    int targetDimensions = 2;

    // Parse command line arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--eps" && i + 1 < argc) {
            eps = std::stod(argv[++i]);
        } else if (arg == "--minPts" && i + 1 < argc) {
            minPts = std::stoi(argv[++i]);
        } else if (arg == "--k" && i + 1 < argc) {
            k = std::stoi(argv[++i]);
        } else if (arg == "--maxIterations" && i + 1 < argc) {
            maxIterations = std::stoi(argv[++i]);
        } else if (arg == "--dimensions" && i + 1 < argc) {
            targetDimensions = std::stoi(argv[++i]);
        } else if (arg == "--input" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        } else {
            throw std::invalid_argument("Invalid argument: " + arg);
        }
    }

    if (inputFile.empty() || outputFile.empty()) {
        printUsage();
        return 1;
    }

    // Read points from input file
    std::vector<Point> points;
    std::ifstream infile(inputFile);
    if (!infile) {
        std::cerr << "Error: Could not open input file " << inputFile << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<double> coords;
        double value;
        while (iss >> value) {
            coords.push_back(value);
        }
        points.emplace_back(coords);
    }
    infile.close();

    // Run the specified algorithm
    try {
        if (algorithm == "dbscan") {
            std::vector<Point> result = dbscan(points, eps, minPts);

            // Write results to output file
            std::ofstream outfile(outputFile);
            if (!outfile) {
                std::cerr << "Error: Could not open output file " << outputFile << std::endl;
                return 1;
            }
            for (const auto& point : result) {
                for (size_t i = 0; i < point.coordinates.size(); ++i) {
                    outfile << point.coordinates[i] << (i < point.coordinates.size() - 1 ? " " : "");
                }
                outfile << " " << point.cluster << "\n";
            }
            outfile.close();
        } else if (algorithm == "kmeans") {
            std::vector<Point> result = kmeans(points, k, maxIterations);

            // Write results to output file
            std::ofstream outfile(outputFile);
            if (!outfile) {
                std::cerr << "Error: Could not open output file " << outputFile << std::endl;
                return 1;
            }
            for (const auto& point : result) {
                for (size_t i = 0; i < point.coordinates.size(); ++i) {
                    outfile << point.coordinates[i] << (i < point.coordinates.size() - 1 ? " " : "");
                }
                outfile << " " << point.cluster << "\n";
            }
            outfile.close();
        } else if (algorithm == "pca") {
            if (targetDimensions <= 0) {
                throw std::invalid_argument("PCA requires a positive target dimension.");
            }
            std::vector<Point> reducedPoints = PCA::reduceDimensions(points, targetDimensions);

            // Write results to output file
            std::ofstream outfile(outputFile);
            if (!outfile) {
                std::cerr << "Error: Could not open output file " << outputFile << std::endl;
                return 1;
            }
            for (const auto& point : reducedPoints) {
                for (size_t i = 0; i < point.coordinates.size(); ++i) {
                    outfile << point.coordinates[i] << (i < point.coordinates.size() - 1 ? " " : "");
                }
                outfile << "\n";
            }
            outfile.close();
        } else {
            throw std::invalid_argument("Invalid algorithm name: " + algorithm);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

