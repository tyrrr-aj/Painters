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

double Point::distance_squared(Point a, Point b) {
    return (a.X - b.X, 2) + pow(a.Y - b.Y, 2);
}

double Point::distance(Point a, Point b) {
    return sqrt(distance_squared(a, b));
}

Point Point::operator-(Point other) {
    return Point(other.X - this->X, other.Y - this->Y);
}