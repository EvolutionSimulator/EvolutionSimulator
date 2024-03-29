#ifndef COLLISION_FUNCTIONS_H
#define COLLISION_FUNCTIONS_H

#include <vector>

#include "core/geometry_primitives.h"
/*!
 * @file collisions.h
 *
 * @brief Defines the functions for collision detection functions in a
 * simulation environment.
 */

bool CollisionCircleCircle(const double& tolerance,
                           const std::pair<double, double>& center1,
                           const double& radius1,
                           const std::pair<double, double>& center2,
                           const double& radius2);
bool CollisionCircleLine(const double& tolerance,
                         const std::pair<double, double>& center,
                         const double& radius,
                         const std::pair<double, double>& endpoint1,
                         const std::pair<double, double>& endpoint2);
bool CollisionLineLine(const double tolerance,
                       const std::pair<double, double> line1_endpoint1,
                       const std::pair<double, double> line1_endpoint2,
                       const std::pair<double, double> line2_endpoint1,
                       const std::pair<double, double> line2_endpoint2);
bool CollisionCircleRectangle(const double tolerance,
                              const std::pair<double, double> center,
                              const double radius,
                              const std::pair<double, double> vertex1,
                              const std::pair<double, double> vertex2,
                              const std::pair<double, double> vertex3,
                              const std::pair<double, double> vertex4);
bool CollisionRectangleRectangle(const double tolerance,
                                 const std::pair<double, double> rect1_vertex1,
                                 const std::pair<double, double> rect1_vertex2,
                                 const std::pair<double, double> rect1_vertex3,
                                 const std::pair<double, double> rect1_vertex4,
                                 const std::pair<double, double> rect2_vertex1,
                                 const std::pair<double, double> rect2_vertex2,
                                 const std::pair<double, double> rect2_vertex3,
                                 const std::pair<double, double> rect2_vertex4);

bool IsGridCellPotentiallyInsideCone(Point grid_point, double grid_cell_size,
                                     Point cone_center, double cone_radius,
                                     OrientedAngle cone_left_boundary,
                                     OrientedAngle cone_right_boundary, double map_width, double map_heigth);

std::vector<std::pair<int, int>> SupercoverBresenhamLine(int x0, int y0, int x1, int y1);
#endif  // COLLISION_FUNCTIONS_H
