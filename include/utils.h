#ifndef UTILS_H
#define UTILS_H

#include "point.h"
#include <cmath>

// Euclidean distance function for n-dimensional data
inline double euclideanDistance(const Point& a, const Point& b) {
    if (a.coordinates.size() != b.coordinates.size()) {
        throw std::invalid_argument("Points must have the same dimensionality.");
    }
    double sum = 0.0;
    for (size_t i = 0; i < a.coordinates.size(); ++i) {
        sum += (a.coordinates[i] - b.coordinates[i]) * (a.coordinates[i] - b.coordinates[i]);
    }
    return sqrt(sum);
}

#endif // UTILS_H

