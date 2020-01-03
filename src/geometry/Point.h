#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Point {
    public:
        Point(double X, double Y);
        Point(Vector v);
        double X;
        double Y;
        
};

#endif