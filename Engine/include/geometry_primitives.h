#pragma once
#include <utility>

class Point {
 public:
  Point(double x, double y);
  Point(std::pair<double, double> coordinates);
  double GetX() const;
  double GetY() const;
  double dist(const Point &other) const;
  Point operator-(const Point &other) const;

 private:
  double x_, y_;
};

class OrientedAngle {
 public:
  OrientedAngle(double angle);
  OrientedAngle(const Point &from, const Point &to);
  double GetAngle() const;
  OrientedAngle operator+(const OrientedAngle &other) const;
  OrientedAngle operator-(const OrientedAngle &other) const;

  double AngleDistanceToCone(const OrientedAngle &left_boundary,
                             const OrientedAngle &right_boundary) const;
  double IsInsideCone(const OrientedAngle &left_boundary,
                      const OrientedAngle &right_boundary) const;

 private:
  void Normalize();
  double angle_;
};