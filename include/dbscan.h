#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include "point.h"

std::vector<Point> dbscan(std::vector<Point>& points, double eps, int minPts);

#endif // DBSCAN_H
