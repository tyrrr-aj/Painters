#ifndef LINES_H
#define LINES_H

#include "Point.h"

namespace lines
{
    double getDistanceBetweenLines(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end);
    double getDistanceBetweenPointAndLine(Point point, Point line_start, Point line_end);
}

#endif