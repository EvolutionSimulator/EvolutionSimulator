#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "entity.h"

class Pheromone : public Entity
{
public:
    Pheromone(int type, double x_coord, double y_coord, double size);

    int GetPheromoneType();

private:
    int pheromone_type_;
};

#endif // PHEROMONE_H
