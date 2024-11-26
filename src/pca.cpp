#include "pca.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <Eigen/Dense>
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Eigen;

std::vector<Point> PCA::reduceDimensions(const std::vector<Point>& points, int targetDimensions) {
    if (points.empty()) {
        throw std::invalid_argument("PCA: Input points cannot be empty.");
    }
    if (targetDimensions <= 0 || targetDimensions > points[0].coordinates.size()) {
        throw std::invalid_argument("PCA: Target dimensions must be between 1 and the number of input dimensions.");
    }

    int numPoints = points.size();
    int numDimensions = points[0].coordinates.size();

    // Convert points to an Eigen matrix
    MatrixXd data(numPoints, numDimensions);

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < numPoints; ++i) {
        for (int j = 0; j < numDimensions; ++j) {
            data(i, j) = points[i].coordinates[j];
        }
    }

    // Mean-center the data
    VectorXd mean(numDimensions);
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int j = 0; j < numDimensions; ++j) {
        mean(j) = data.col(j).mean();
    }

    MatrixXd centeredData(numPoints, numDimensions);
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < numPoints; ++i) {
        for (int j = 0; j < numDimensions; ++j) {
            centeredData(i, j) = data(i, j) - mean(j);
        }
    }

    // Compute the covariance matrix
    MatrixXd covariance = (centeredData.transpose() * centeredData) / (numPoints - 1);

    // Perform eigen decomposition
    SelfAdjointEigenSolver<MatrixXd> solver(covariance);
    if (solver.info() != Success) {
        throw std::runtime_error("PCA: Eigen decomposition failed.");
    }

    // Get the top targetDimensions eigenvectors
    MatrixXd eigenvectors = solver.eigenvectors().rightCols(targetDimensions);

    // Project data onto the top eigenvectors
    MatrixXd reducedData(numPoints, targetDimensions);
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < numPoints; ++i) {
        for (int j = 0; j < targetDimensions; ++j) {
            reducedData(i, j) = centeredData.row(i) * eigenvectors.col(j);
        }
    }

    // Convert reduced data back to Point objects
    std::vector<Point> reducedPoints;
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        std::vector<Point> localReducedPoints;
        #ifdef _OPENMP
        #pragma omp for nowait
        #endif
        for (int i = 0; i < numPoints; ++i) {
            std::vector<double> newCoords(targetDimensions);
            for (int j = 0; j < targetDimensions; ++j) {
                newCoords[j] = reducedData(i, j);
            }
            localReducedPoints.emplace_back(newCoords);
        }
        #ifdef _OPENMP
        #pragma omp critical
        #endif
        reducedPoints.insert(reducedPoints.end(), localReducedPoints.begin(), localReducedPoints.end());
    }

    return reducedPoints;
}

