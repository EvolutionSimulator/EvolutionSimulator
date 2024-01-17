#ifndef PHEROMONESYSTEM_H
#define PHEROMONESYSTEM_H

#include <vector>

#include "alive_entity.h"

class PheromoneSystem : virtual public AliveEntity
{
public:
    PheromoneSystem(neat::Genome gemone, Mutable mutables);

    void GetPheromonesDensity(std::vector<std::vector<std::vector<Entity *>>> &grid,
                              double GridCellSize) const;

    void EmitPheromones(std::vector<double> emissions);




private:
    std::vector<char> pheromone_types;
    std::vector<double> pheromone_densities_;
    std::vector<double> emissions_;


};

#endif
