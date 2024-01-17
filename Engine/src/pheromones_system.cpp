#include "pheromones_system.h"

PheromoneSystem::PheromoneSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables) {
    for (BrainModule module : genome.GetModules()){
        if (module.GetModuleId() == 2){
            pheromone_types_.push_back(module.GetType());
        }
    }
    pheromone_densities_ = std::vector<double> (pheromone_types_.size(), 0.0);
    pheromone_emissions_ = std::vector<double> (pheromone_types_.size(), 0.0);
}

void PheromoneSystem::GetPheromonesDensity(std::vector<std::vector<std::vector<Entity *>>> &grid,
                                           double GridCellSize) const {
    int x_grid = static_cast<int>(x_coord_ / GridCellSize);
    int y_grid = static_cast<int>(y_coord_ / GridCellSize);

    std::vector<std::pair<int, int>> cells;
    int reach = std::floor(size_/GridCellSize);
    for (int i = -reach; i <= reach; i++){
        for (int j = -reach; j <= reach; j++){
            cells.push_back({x_grid + i, y_grid + j});
        }
    }

    for (std::pair<int, int> cell : cells){
        for (int type : pheromone_types_){
            //check for pheromones in the grid
        }
    }
}
