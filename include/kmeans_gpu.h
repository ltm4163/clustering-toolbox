#ifndef KMEANS_GPU_H
#define KMEANS_GPU_H

#include "point.h"
#include <vector>

std::vector<Point> kmeans_gpu(const std::vector<Point>& points, int k, int maxIterations);

#endif // KMEANS_GPU_H

