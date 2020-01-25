#ifndef LINES_H
#define LINES_H

#include <cmath>
#include <algorithm>

#include "Point.h"
#include "Vector.h"
#include "linear_functions.h"

namespace lines
{
    double getDistanceBetweenLines(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end);
    double getDistanceBetweenPointAndLine(Point point, Point line_start, Point line_end);
    bool check_if_intersect(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end);
}

#endif