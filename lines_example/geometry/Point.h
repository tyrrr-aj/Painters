#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Vector;

class Point {
    public:
        Point() {}
        Point(double X, double Y);
        Point(Vector v);
        double X;
        double Y;
        
        static double distance(Point a, Point b);
        static double distance_squared(Point a, Point b);

        void operator=(Point other);
        Point operator-(Point other);
};

#endif