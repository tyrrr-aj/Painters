#ifndef LINEAR_FUNCTIONS_H
#define LINEAR_FUNCTIONS_H

#include "Point.h"

namespace linear_functions {
    double getDirectionCoefficient(Point A, Point B);
    double getConstant(Point point, double direction_coefficient);
}

#endif