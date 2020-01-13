#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Point {
    public:
        Point() {}
        Point(double X, double Y);
        Point(Vector v);
        double X;
        double Y;

        void operator=(Point other);
        
};

#endif