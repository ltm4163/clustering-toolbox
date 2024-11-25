#ifndef POINT_H
#define POINT_H

#include <cmath> // For floating-point comparisons
#include <iostream>

struct Point {
    double x, y;
    int cluster;

    // Default constructor
    #ifdef __CUDACC__
    __device__ __host__
    #endif
    Point(double _x = 0.0, double _y = 0.0, int _cluster = -1)
        : x(_x), y(_y), cluster(_cluster) {}

    // Equality operator
    #ifdef __CUDACC__
    __device__ __host__
    #endif
    bool operator==(const Point& other) const {
        const double epsilon = 1e-9; // Tolerance for floating-point comparisons
        return (fabs(x - other.x) < epsilon && fabs(y - other.y) < epsilon && cluster == other.cluster);
    }

    // Optional: Print function for debugging on the host
    void print() const {
        std::cout << "Point(" << x << ", " << y << ", cluster=" << cluster << ")" << std::endl;
    }
};

#endif // POINT_H

