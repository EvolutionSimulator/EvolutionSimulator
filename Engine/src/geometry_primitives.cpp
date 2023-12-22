#include "geometry_primitives.h"

#include <algorithm>
#include <cmath>

#include "config.h"

Point::Point(double x, double y) : x_(x), y_(y) {}

Point::Point(std::pair<double, double> coordinates)
    : x_(coordinates.first), y_(coordinates.second) {}

double Point::GetX() const { return x_; }

double Point::GetY() const { return y_; }

double Point::dist(const Point &other) const {
  return hypot(x_ - other.GetX(), y_ - other.GetY());
}

Point Point::operator-(const Point &other) const {
  return Point(x_ - other.GetX(), y_ - other.GetY());
}

OrientedAngle::OrientedAngle(double angle) : angle_(angle) { Normalize(); }
OrientedAngle::OrientedAngle(const Point &from, const Point &to)
    : angle_(atan2(to.GetY() - from.GetY(), to.GetX() - from.GetX())) {
  Normalize();
}

double OrientedAngle::GetAngle() const { return angle_; }

OrientedAngle OrientedAngle::operator+(const OrientedAngle &other) const {
  return OrientedAngle(angle_ + other.GetAngle());
}

OrientedAngle OrientedAngle::operator-(const OrientedAngle &other) const {
  return OrientedAngle(angle_ - other.GetAngle());
}

void OrientedAngle::Normalize() { angle_ = remainder(angle_, 2 * M_PI); }

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

  double distance_to_left_boundary = abs(angle_ - left_boundary_angle);
  distance_to_left_boundary =
      std::min(distance_to_left_boundary, 2 * M_PI - distance_to_left_boundary);

  double distance_to_right_boundary = abs(angle_ - right_boundary_angle);
  distance_to_right_boundary = std::min(distance_to_right_boundary,
                                        2 * M_PI - distance_to_right_boundary);

  return std::min(distance_to_left_boundary, distance_to_right_boundary);
}

double OrientedAngle::IsInsideCone(const OrientedAngle &left_boundary,
                                   const OrientedAngle &right_boundary) const {
  return AngleDistanceToCone(left_boundary, right_boundary) <
         settings::engine::EPS;
}