#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "point.h"

std::vector<Point> kmeans(std::vector<Point>& points, int k, int maxIterations);

#endif // KMEANS_H
