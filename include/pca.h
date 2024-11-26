#ifndef PCA_H
#define PCA_H

#include <vector>
#include <string>
#include "point.h"

class PCA {
public:
    static std::vector<Point> reduceDimensions(const std::vector<Point>& points, int targetDimensions);
};

#endif

