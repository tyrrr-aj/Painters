#include "linear_functions.h"

double linear_functions::getDirectionCoefficient(Point A, Point B) {
    return (B.Y - A.Y) / (A.X - B.X);
}

double linear_functions::getConstant(Point point, double direction_coefficient) {
    return point.Y - direction_coefficient * point.X;
}