#include "pheromone.h"

Pheromone::Pheromone(int type, double x_coord, double y_coord, double size)
    : Entity(x_coord, y_coord, size), pheromone_type_(type)
{
    color_hue_ = type/16;
}

int Pheromone::GetPheromoneType() { return pheromone_type_; }

