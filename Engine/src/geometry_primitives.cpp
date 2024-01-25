#include "geometry_primitives.h"

#include <algorithm>
#include <cmath>

#include "settings.h"

/*!
 * @brief Constructs a Point using individual x and y coordinates.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 */
Point::Point(double x, double y) : x_(x), y_(y) {}

/*!
 * @brief Constructs a Point from a pair of coordinates.
 * @param coordinates A pair representing the x and y coordinates.
 */
Point::Point(std::pair<double, double> coordinates)
    : x_(coordinates.first), y_(coordinates.second) {}

/*!
 * @brief Accessor for the x-coordinate.
 * @return The x-coordinate of the Point.
 */
double Point::GetX() const { return x_; }

/*!
 * @brief Accessor for the y-coordinate.
 * @return The y-coordinate of the Point.
 */
double Point::GetY() const { return y_; }

/*!
 * @brief Calculates the Euclidean distance to another Point.
 * @param other The other Point to which the distance is calculated.
 * @return The Euclidean distance as a double.
 */
double Point::dist(const Point &other, double map_width, double map_heigth) const {
  const double x_diff = fabs(x_ - other.GetX());
  const double y_diff = fabs(y_ - other.GetY());
  return std::hypot(fmin(x_diff, map_width - x_diff),
                    fmin(y_diff, map_heigth - y_diff));
}

/*!
 * @brief Overloads the subtraction operator to handle Point arithmetic.
 * @param other The other Point to subtract from this Point.
 * @return A new Point representing the result of the subtraction.
 */
Point Point::operator-(const Point &other) const {
  return Point(x_ - other.GetX(), y_ - other.GetY());
}

/*!
 * @brief Constructs an OrientedAngle from a direct angle value.
 * @param angle The angle value in radians.
 */
OrientedAngle::OrientedAngle(double angle) : angle_(angle) { Normalize(); }

/*!
 * @brief Constructs an OrientedAngle based on the direction from one Point to
 * another.
 * @param from The starting Point.
 * @param to The ending Point.
 */
OrientedAngle::OrientedAngle(const Point &from, const Point &to, double map_width, double map_heigth) {

  auto diff_x = to.GetX() - from.GetX();
  if (diff_x > map_width/2)
  {
    diff_x -=  map_width;
  }
  if (diff_x < - map_width/2)
  {
    diff_x +=  map_width;
  }

  auto diff_y = to.GetY() - from.GetY();
  if (diff_y > map_heigth/2)
  {
    diff_y -=  map_heigth;
  }
  if (diff_y < - map_heigth/2)
  {
    diff_y +=  map_heigth;
  }
  angle_ = atan2(diff_y, diff_x);
  Normalize();
}

/*!
 * @brief Accessor for the angle.
 * @return The angle value in radians.
 */
double OrientedAngle::GetAngle() const { return angle_; }

/*!
 * @brief Overloads addition operator to handle Oriented Angles.
 * @param other The other OrientedAngle to add.
 * @return A new OrientedAngle representing the sum of this angle and the other.
 */
OrientedAngle OrientedAngle::operator+(const OrientedAngle &other) const {
  return OrientedAngle(angle_ + other.GetAngle());
}

/*!
 * @brief Overloads subtraction operator to handle Oriented Angles.
 * @param other The other OrientedAngle to subtract from this angle.
 * @return A new OrientedAngle representing the difference between this angle
 * and the other.
 */
OrientedAngle OrientedAngle::operator-(const OrientedAngle &other) const {
  return OrientedAngle(angle_ - other.GetAngle());
}

/*!
 * @brief Normalizes the angle to be within [-π, π).
 */
void OrientedAngle::Normalize() { angle_ = remainder(angle_, 2 * M_PI); }

/*!
 * @brief Calculates the minimum angle between this angle and a cone defined by
 * two boundaries.
 * @param left_boundary The left boundary of the cone.
 * @param right_boundary The right boundary of the cone.
 * @return The minimum angle distance to the cone.
 */
double OrientedAngle::AngleDistanceToCone(
    const OrientedAngle &left_boundary,
    const OrientedAngle &right_boundary) const {
  double left_boundary_angle = left_boundary.GetAngle();
  double right_boundary_angle = right_boundary.GetAngle();

  if (left_boundary_angle < right_boundary_angle &&
      (left_boundary_angle <= angle_ && angle_ <= right_boundary_angle)) {
    return 0.0;
  }
  if (right_boundary_angle < left_boundary_angle &&
      (angle_ <= right_boundary_angle || left_boundary_angle <= angle_)) {
    return 0.0;
  }

  double distance_to_left_boundary = std::abs(angle_ - left_boundary_angle);
  distance_to_left_boundary =
      std::min(distance_to_left_boundary, 2 * M_PI - distance_to_left_boundary);

  double distance_to_right_boundary = std::abs(angle_ - right_boundary_angle);
  distance_to_right_boundary = std::min(distance_to_right_boundary,
                                        2 * M_PI - distance_to_right_boundary);

  return std::min(distance_to_left_boundary, distance_to_right_boundary);
}

/*!
 * @brief Determines if the angle is inside a cone defined by two boundaries.
 * @param left_boundary The left boundary of the cone.
 * @param right_boundary The right boundary of the cone.
 * @return True if the angle is inside the cone, otherwise false.
 */
double OrientedAngle::IsInsideCone(const OrientedAngle &left_boundary,
                                   const OrientedAngle &right_boundary) const {
  return AngleDistanceToCone(left_boundary, right_boundary) <
         SETTINGS.engine.eps;
}
