#ifndef DBSCAN_GPU_H
#define DBSCAN_GPU_H

#include "point.h"
#include <vector>

std::vector<Point> dbscan_gpu(const std::vector<Point>& points, double eps, int minPts);

#endif // DBSCAN_GPU_H

