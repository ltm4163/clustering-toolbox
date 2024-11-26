#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <vector>

struct Point {
    std::vector<double> coordinates;
    int cluster;

    // Default constructor
    Point(const std::vector<double>& coords = {}, int _cluster = -1)
        : coordinates(coords), cluster(_cluster) {}

    // Equality operator for checking convergence in K-Means
    bool operator==(const Point& other) const {
        if (coordinates.size() != other.coordinates.size()) return false;
        for (size_t i = 0; i < coordinates.size(); ++i) {
            if (fabs(coordinates[i] - other.coordinates[i]) > 1e-9) {
                return false;
            }
        }
        return cluster == other.cluster;
    }

    // Print function for debugging
    void print() const {
        std::cout << "Point(";
        for (size_t i = 0; i < coordinates.size(); ++i) {
            std::cout << coordinates[i];
            if (i < coordinates.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ", cluster=" << cluster << ")" << std::endl;
    }
};

#endif // POINT_H

