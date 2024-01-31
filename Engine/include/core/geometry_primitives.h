#pragma once
#include <utility>

/*!
 * @file geometry_primitives.h
 * @brief Defines the Point and OrientedAngle classes for geometric operations.
 *
 * @details This file contains the definitions of Point and OrientedAngle classes,
 * which are fundamental in performing various geometric computations and
 * transformations. The Point class represents a 2D point with x and y coordinates
 * and offers functionality such as distance calculation and operator overloading
 * for arithmetic. The OrientedAngle class encapsulates an angle measurement and
 * provides methods for angle arithmetic and geometric queries like determining
 * the position relative to a cone.
 */

/*!
 * @class Point
 * @brief Represents a 2-dimensional point with x and y coordinates.
 *
 * @details This class models a point in 2D space with double precision for its
 * x and y coordinates. It supports initialization from direct coordinates or
 * a pair of values, and provides methods for accessing individual coordinates,
 * calculating distance to another point, and arithmetic operations with other
 * points.
 */
class Point {
 public:
  Point(double x, double y);
  Point(std::pair<double, double> coordinates);
  double GetX() const;
  double GetY() const;
  double dist(const Point &other, double map_width, double map_heigth) const;
  Point operator-(const Point &other) const;

 private:
  double x_, y_;  /*! < The x and y-coordinates of the Point. */
};


/*!
 * @class OrientedAngle
 * @brief Represents an angle measurement between [-pi,pi], providing various utilities for angle
 * arithmetic and geometric comparisons.
 *
 * @details This class encapsulates an angle value, offering functionalities such
 * as adding and subtracting angles, normalizing the angle within the range [-pi,pi],
 * and geometric queries like checking if an angle is within a specified cone.
 * It's particularly useful in scenarios involving rotational dynamics or angular
 * positioning.
 */
class OrientedAngle {
 public:
  OrientedAngle(double angle);
  OrientedAngle(const Point &from, const Point &to, double map_width, double map_heigth);
  double GetAngle() const;
  OrientedAngle operator+(const OrientedAngle &other) const;
  OrientedAngle operator-(const OrientedAngle &other) const;

  double AngleDistanceToCone(const OrientedAngle &left_boundary,
                             const OrientedAngle &right_boundary) const;
  double IsInsideCone(const OrientedAngle &left_boundary,
                      const OrientedAngle &right_boundary) const;

 private:
  void Normalize();  /*!< Normalizes the angle value between [-pi,pi] */
  double angle_; /*!< The encapsulated angle value */
};
