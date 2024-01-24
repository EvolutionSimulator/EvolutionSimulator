#include "collision_functions.h"

#include <cmath>
#include <assert.h>

#include "geometry_primitives.h"
#include "settings.h"

/*!
 * @file collision_utils.cpp
 * @brief Collision Detection and Distance Calculation Utilities.
 *
 * @details This file provides a collection of utility functions used for
 * collision detection and distance calculations within a simulation
 * environment. These functions are critical for determining interactions
 *          between various entities within the simulation, such as collisions
 * between circles (representing objects or agents) and line segments
 * (potentially representing boundaries or obstacles).
 */

/*!
 * @brief Calculates the Euclidean distance between two points.
 *
 * @details Computes the straight-line distance between two points in a 2D plane
 * using the Euclidean distance formula.
 *
 * @param point1 The first point as a pair of x and y coordinates.
 * @param point2 The second point as a pair of x and y coordinates.
 *
 * @return double The Euclidean distance between the two points.
 */
double EuclideanDistance(const std::pair<double, double>& point1,
                         const std::pair<double, double>& point2) {
  return std::sqrt(std::abs(std::pow(point1.first - point2.first, 2) +
                   std::pow(point1.second - point2.second, 2)));
}

/*!
 * @brief Calculates the shortest distance from a point to a line segment.
 *
 * @details Determines the minimum distance from a given point to a line
 * segment, not an infinite line. If the perpendicular from the point to the
 * line falls outside the line segment, the distance is calculated from the
 * point to the nearest endpoint of the segment.
 *
 * @param point The point for which the distance to the line segment is to be
 * calculated.
 * @param linePoint1 The first endpoint of the line segment.
 * @param linePoint2 The second endpoint of the line segment.
 *
 * @return double The shortest distance from the point to the line segment.
 */
double ShortestDistancePointLineSegment(
    const std::pair<double, double>& point,
    const std::pair<double, double>& linePoint1,
    const std::pair<double, double>& linePoint2) {
  // Calculate coefficients of the line equation Ax + By + C = 0
  double A = linePoint2.second - linePoint1.second;
  double B = linePoint1.first - linePoint2.first;
  double C = linePoint2.first * linePoint1.second -
             linePoint1.first * linePoint2.second;

  // Calculate the distance using the formula for an infinite line
  assert(std::abs(A*A + B*B) != 0);
  double dist = std::fabs(A * point.first + B * point.second + C) /
                std::sqrt(std::abs(A * A + B * B));

  // Calculate the squared length of the line segment
  double lineSegmentLengthSquared = EuclideanDistance(linePoint1, linePoint2) *
                                    EuclideanDistance(linePoint1, linePoint2);

  // Calculate the dot product of vectors (point - linePoint1) and (linePoint2 -
  // linePoint1)
  double dotProduct = ((point.first - linePoint1.first) *
                       (linePoint2.first - linePoint1.first)) +
                      ((point.second - linePoint1.second) *
                       (linePoint2.second - linePoint1.second));

  // Calculate the parameter t (distance from linePoint1 to the closest point on
  // the line)
  double t = dotProduct / lineSegmentLengthSquared;

  // Check if the closest point falls within the line segment
  if (t < 0.0) {  // Closest point is beyond linePoint1
    dist = EuclideanDistance(point, linePoint1);
  } else if (t > 1.0) {  // Closest point is beyond linePoint2
    dist = EuclideanDistance(point, linePoint2);
  }

  return dist;
}

/*!
 * @brief Determines if two circles collide or are within a certain tolerance of
 * each other.
 *
 * @details Checks for collision or proximity between two circles, taking into
 * account a specified tolerance. The circles are considered to be colliding or
 * in proximity if the distance between their centers is less than or equal to
 * the sum of their radii plus the tolerance.
 *
 * @param tolerance The tolerance within which two circles are considered to be
 * colliding or in proximity.
 * @param center1 The center of the first circle.
 * @param radius1 The radius of the first circle.
 * @param center2 The center of the second circle.
 * @param radius2 The radius of the second circle.
 *
 * @return true if the circles collide or are within the specified tolerance,
 * false otherwise.
 */
bool CollisionCircleCircle(const double& tolerance,
                           const std::pair<double, double>& center1,
                           const double& radius1,
                           const std::pair<double, double>& center2,
                           const double& radius2) {
  return (EuclideanDistance(center1, center2) <
          (radius1 + radius2 + tolerance));
}

/*!
 * @brief Determines if a circle collides with or is within a certain tolerance
 * of a line segment.
 *
 * @details Checks for collision or proximity between a circle and a line
 * segment, considering a specified tolerance. The circle and line segment are
 * considered to be colliding or in proximity if the shortest distance from the
 * circle's center to the line segment is less than or equal to the circle's
 * radius plus the tolerance.
 *
 * @param tolerance The tolerance within which the circle and line segment are
 * considered to be colliding or in proximity.
 * @param center The center of the circle.
 * @param radius The radius of the circle.
 * @param linePoint1 The first endpoint of the line segment.
 * @param linePoint2 The second endpoint of the line segment.
 *
 * @return true if the circle and line segment collide or are within the
 * specified tolerance, false otherwise.
 */
bool CollisionCircleLine(const double& tolerance,
                         const std::pair<double, double>& center,
                         const double& radius,
                         const std::pair<double, double>& linePoint1,
                         const std::pair<double, double>& linePoint2) {
  return (ShortestDistancePointLineSegment(center, linePoint1, linePoint2) <
          radius + tolerance);
}

/*!
 * @brief Determines if a grid cell might intersect with or lie inside a
 * specified cone.
 *
 * @details This function evaluates whether a grid cell, represented by a point
 * and its size, could potentially be inside a cone defined by a center point,
 * radius, and angular boundaries. It accounts for edge cases and uses a small
 * epsilon value for precision considerations.
 *
 * @param grid_point The representative point of the grid cell, typically its
 * center.
 * @param grid_cell_size The length of the side of the square grid cell.
 * @param cone_center The center point of the cone.
 * @param cone_radius The radius of the cone from the center point.
 * @param cone_left_boundary The left boundary angle of the cone.
 * @param cone_right_boundary The right boundary angle of the cone.
 * @return True if the grid cell is potentially inside the cone, otherwise
 * false.
 */
bool IsGridCellPotentiallyInsideCone(Point grid_point, double grid_cell_size,
                                     Point cone_center, double cone_radius,
                                     OrientedAngle cone_left_boundary,
                                     OrientedAngle cone_right_boundary,
                                     double map_width,
                                     double map_heigth) {
  auto EPS = SETTINGS.engine.eps;
  double distance = grid_point.dist(cone_center, map_width, map_heigth);
  if (distance < EPS) {
    return true;
  }
  double max_distance_in_cell = sqrt(2) * grid_cell_size;
  if (distance > cone_radius + max_distance_in_cell +
                     SETTINGS.environment.max_food_size + EPS) {
    return false;
  }
  OrientedAngle cell_relative_angle(cone_center, grid_point);
  double angle_distance = cell_relative_angle.AngleDistanceToCone(
      cone_left_boundary, cone_right_boundary);
  if (sin(angle_distance) >
      (max_distance_in_cell + SETTINGS.environment.max_food_size) / distance +
          EPS) {
    return false;
  }
  return true;
}

/*!
 * @brief Generates a supercover of grid cells that a line passes through.
 *
 * @details This function implements a variant of Bresenham's line algorithm.
 * It calculates a supercover of the grid cells that a line between two points
 * (x0, y0) and (x1, y1) passes through on a grid.
 *
 * @param x0 The x-coordinate of the starting point of the line.
 * @param y0 The y-coordinate of the starting point of the line.
 * @param x1 The x-coordinate of the ending point of the line.
 * @param y1 The y-coordinate of the ending point of the line.
 *
 * @return A vector of pairs of integers, each representing a grid cell (x, y)
 * that the line intersects.
 */
std::vector<std::pair<int, int>> SupercoverBresenhamLine(int x0, int y0, int x1,
                                                         int y1) {
  std::vector<std::pair<int, int>> line;

  int dx = x1 - x0;
  int dy = y1 - y0;
  int nx = std::abs(dx);
  int ny = std::abs(dy);
  int sign_x = dx > 0 ? 1 : -1;
  int sign_y = dy > 0 ? 1 : -1;

  int x = x0;
  int y = y0;

  line.push_back({x, y});

  for (int ix = 0, iy = 0; ix < nx || iy < ny;) {
    if ((0.5 + ix) / nx < (0.5 + iy) / ny) {
      x += sign_x;
      ix++;
    } else {
      y += sign_y;
      iy++;
    }
    line.push_back({x, y});
  }

  return line;
}
