#include <cmath>
#include "collisions.h"

double EuclideanDistance(const std::pair<double, double>& point1, const std::pair<double, double>& point2){
    return std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2));
}

double ShortestDistancePointLineSegment(const std::pair<double, double>& point, const std::pair<double, double>& linePoint1, const std::pair<double, double>& linePoint2) {
    // Calculate coefficients of the line equation Ax + By + C = 0
    double a, b, c;
    if (linePoint2.first - linePoint1.first == 0){
        a = 1.0;
        b = 0.0;
        c = -linePoint1.first;
    }
    else{
        a = -(linePoint2.second - linePoint1.second)/(linePoint2.first - linePoint1.first);
        b = 1.0;
        c = linePoint1.second + a * linePoint1.first;
    }

    const double distance = std::abs(a * point.first + b * point.second + c) / std::sqrt(std::pow(a, 2) + std::pow(b, 2));
    double x = (b * (b * point.first - a * point.second) - a * c) / (std::pow(a, 2) + std::pow(b, 2));
    //double y = (-a * (b * point.first - a * point.second) - b * c) / (std::pow(a, 2) + std::pow(b, 2));

    if (x <= std::max(linePoint1.first, linePoint2.first) && x >= std::min(linePoint1.first, linePoint2.first)){
        return distance;
    }

    return std::min(EuclideanDistance(point, linePoint1), EuclideanDistance(point, linePoint2));
}

bool CollisionCircleCircle(const double& tolerance, const std::pair<double, double>& center1, const double& radius1, const std::pair<double, double>& center2, const double& radius2){
    return (EuclideanDistance(center1, center2)< (radius1 + radius2 + tolerance));
}

bool CollisionCircleLine(const double& tolerance, const std::pair<double, double>& center, const double& radius,
                         const std::pair<double, double>& linePoint1, const std::pair<double, double>& linePoint2){
    return (ShortestDistancePointLineSegment(center, linePoint1, linePoint2)<radius+tolerance);
}
