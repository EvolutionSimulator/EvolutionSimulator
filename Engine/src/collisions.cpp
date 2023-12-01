#include <cmath>
#include <iostream>
#include "collisions.h"

double EuclideanDistance(const std::pair<double, double>& point1, const std::pair<double, double>& point2){
    return std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2));
}

double ShortestDistancePointLineSegment(const std::pair<double, double>& point, const std::pair<double, double>& linePoint1, const std::pair<double, double>& linePoint2) {
    // Calculate coefficients of the line equation Ax + By + C = 0
    double A = linePoint2.second - linePoint1.second;
    double B = linePoint1.first - linePoint2.first;
    double C = linePoint1.first * linePoint2.second - linePoint2.first * linePoint1.second;

    // Calculate the distance using the formula for an infinite line
    double dist = std::abs(A * point.first + B * point.second + C) / std::sqrt(A * A + B * B);

    // Calculate the squared length of the line segment
    double lineSegmentLengthSquared = EuclideanDistance(linePoint1, linePoint2) * EuclideanDistance(linePoint1, linePoint2);

    // Calculate the dot product of vectors (point - linePoint1) and (linePoint2 - linePoint1)
    double dotProduct = ((point.first - linePoint1.first) * (linePoint2.first - linePoint1.first)) +
                        ((point.second - linePoint1.second) * (linePoint2.second - linePoint1.second));

    // Calculate the parameter t (distance from linePoint1 to the closest point on the line)
    double t = dotProduct / lineSegmentLengthSquared;

    // Check if the closest point falls within the line segment
    if (t < 0.0) { // Closest point is beyond linePoint1
        dist = EuclideanDistance(point, linePoint1);
    } else if (t > 1.0) { // Closest point is beyond linePoint2
        dist = EuclideanDistance(point, linePoint2);
    }

    return dist;
}

bool CollisionCircleCircle(const double& tolerance, const std::pair<double, double>& center1, const double& radius1, const std::pair<double, double>& center2, const double& radius2){
    return (EuclideanDistance(center1, center2)< (radius1 + radius2 + tolerance));
}

bool CollisionCircleLine(const double& tolerance, const std::pair<double, double>& center, const double& radius,
                         const std::pair<double, double>& linePoint1, const std::pair<double, double>& linePoint2){
    return (ShortestDistancePointLineSegment(center, linePoint1, linePoint2)<radius+tolerance);
}
