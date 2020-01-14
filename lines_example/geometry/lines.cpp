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
  const double t = std::max(0.0, std::min(1.0, Vector(point - line_start).scalar_product(Vector(line_end - line_start)) / segment_length_squared));
  const Vector projection = Vector(line_start) + Vector(line_end - line_start) * t;  // Projection falls on the segment
  return Point::distance(point, Point(projection));
}

bool lines::check_if_intersect(Point first_line_start, Point first_line_end, Point second_line_start, Point second_line_end) {

}