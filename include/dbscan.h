#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include "point.h"

std::vector<int> dbscan(const std::vector<Point>& points, double epsilon, int minPoints);

#endif // DBSCAN_H
