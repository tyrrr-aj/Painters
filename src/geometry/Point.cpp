#include "Point.h"

Point::Point(double X, double Y) {
    this->X = X;
    this->Y = Y;
}

Point::Point(Vector v) {
    this->X = v.X;
    this->Y = v.Y;
}

void Point::operator=(Point other){
    this->X = other.X;
    this->Y = other.Y;
}