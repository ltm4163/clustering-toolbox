#ifndef UTILS_H
#define UTILS_H

#include "point.h"
#include <cmath>

// Euclidean distance function compatible with both CUDA and C++
#ifdef __CUDACC__
__device__ __host__
#endif
inline double euclideanDistance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

#endif // UTILS_H
