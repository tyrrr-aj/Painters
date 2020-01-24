#include "lines.h"

double lines::getDistanceBetweenLines(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end) {
    if (check_if_intersect(first_line_start, first_line_end, second_line_start, second_line_end))
        return 0.0;
    double distances[4];
    distances[0] = getDistanceBetweenPointAndLine(first_line_start, second_line_start, second_line_end);
    distances[1] = getDistanceBetweenPointAndLine(first_line_end, second_line_start, second_line_end);
    distances[2] = getDistanceBetweenPointAndLine(second_line_start, first_line_start, first_line_end);
    distances[3] = getDistanceBetweenPointAndLine(second_line_end, first_line_start, first_line_end);
    return *std::min_element(distances, distances + 3);
}

double lines::getDistanceBetweenPointAndLine(Point point, Point line_start, Point line_end) {
    // Return minimum distance between line segment vw and point p
    const double segment_length_squared = Point::distance_squared(line_start, line_end);
    if (segment_length_squared == 0.0) return Point::distance_squared(line_start, point);
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    Vector *vec1 = new Vector(point - line_start);
    Vector *vec2 = new Vector(line_end - line_start);
    const double t = std::max(0.0, std::min(1.0, vec1->scalar_product(*vec2) / segment_length_squared));
    const Vector projection = Vector(line_start) + Vector(line_end - line_start) * t;  // Projection falls on the segment
    delete vec1;
    delete vec2;
    return Point::distance(point, Point(projection));
}

bool lines::check_if_intersect(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end) {
    using namespace linear_functions;

    // special case - vertical line
    double epsilon = 0.0001;
    if (first_line_start.X - first_line_end.X < epsilon) {
        if (second_line_start.X - second_line_end.X < epsilon) {
            return std::min(first_line_start.Y, first_line_end.Y) > std::max(second_line_start.Y, second_line_end.Y)
                || std::max(first_line_start.Y, first_line_end.Y) < std::min(second_line_start.Y, second_line_end.Y);
        }
        else {
            double a2 = getDirectionCoefficient(second_line_start, second_line_end);
            double b2 = getConstant(second_line_start, a2);
            double y2 = a2 * first_line_start.X + b2;
            return y2 < std::max(first_line_start.Y, first_line_end.Y) && y2 > std::min(first_line_start.Y, first_line_end.Y);
        }
    }
    else {
        double a1 = getDirectionCoefficient(first_line_start, first_line_end);
        double b1 = getConstant(first_line_start, a1);
        double y1 = a1 * first_line_start.X + b1;
        return y1 < std::max(first_line_start.Y, first_line_end.Y) && y1 > std::min(first_line_start.Y, first_line_end.Y);
    }

    // general case
    double a1 = getDirectionCoefficient(first_line_start, first_line_end);
    double b1 = getConstant(first_line_start, a1);

    double a2 = getDirectionCoefficient(second_line_start, second_line_end);
    double b2 = getConstant(second_line_start, a2);

    double x = (b2 - b1) / (a1 - a2);
    double y = a1*x + b1;

    return x >= std::min(first_line_start.X, first_line_end.X) && x <= std::max(first_line_start.X, first_line_end.X)
        && x >= std::min(second_line_start.X, second_line_end.X) && x <= std::max(second_line_start.X, second_line_end.X)
        && y >= std::min(first_line_start.Y, first_line_end.Y) && y <= std::max(first_line_start.Y, first_line_end.Y)
        && y >= std::min(second_line_start.Y, second_line_end.Y) && y <= std::max(second_line_start.Y, second_line_end.Y);
}