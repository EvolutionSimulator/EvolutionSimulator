#ifndef PHEROMONESYSTEM_H
#define PHEROMONESYSTEM_H

#include <vector>
#include <memory>

#include "alive_entity.h"
#include "pheromone.h"

class PheromoneSystem : virtual public AliveEntity
{
public:
    PheromoneSystem(neat::Genome gemone, Mutable mutables);

    void ProcessPheromoneDetection(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                                   double GridCellSize);

    std::vector<double> GetPheromoneDensities(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                                              double GridCellSize) const ;

    std::vector<std::shared_ptr<Pheromone>> EmitPheromones(double deltaTime);

protected:
    std::vector<int> pheromone_types_;
    std::vector<double> pheromone_densities_;
    std::vector<double> pheromone_emissions_;
};

#endif
