#include "collisions.h"

#include <cmath>
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
  return std::sqrt(std::pow(point1.first - point2.first, 2) +
                   std::pow(point1.second - point2.second, 2));
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
  double dist = std::fabs(A * point.first + B * point.second + C) /
                std::sqrt(A * A + B * B);

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
