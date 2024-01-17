#include "pheromones_system.h"

#include "settings.h"

PheromoneSystem::PheromoneSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables), pheromone_densities_(16, 0),
      pheromone_emissions_(16, 0), pheromone_types_(16,0) {//We use 16 as that is the established number of pheromones avaialable atm
    for (BrainModule module : genome.GetModules()){
        if (module.GetModuleId() == 2){
            pheromone_types_.at(module.GetType()) = 1;
        }
    }
}

std::vector<double> PheromoneSystem::GetPheromoneDensities(
        std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
        double GridCellSize) const {
    int x_grid = static_cast<int>(x_coord_ / GridCellSize);
    int y_grid = static_cast<int>(y_coord_ / GridCellSize);

    int grid_width = grid.size();
    int grid_height = grid[0].size();

    std::vector<double> pheromone_densities(16, 0);

    std::vector<std::pair<int, int>> cells;
    int reach = std::floor(size_/GridCellSize);
    for (int i = -reach; i <= reach; i++){
      for (int j = -reach; j <= reach; j++){
        cells.push_back({(x_grid + i) % grid_width, (y_grid + j) % grid_height});
      }
    }

    for (std::pair<int, int> cell : cells){
      for (std::shared_ptr<Entity> entity : grid[cell.first][cell.second]){
        std::shared_ptr<Pheromone> pheromone = std::dynamic_pointer_cast<Pheromone>(entity);
        if (pheromone && pheromone_types_.at(pheromone->GetPheromoneType()) == 1){
            pheromone_densities.at(pheromone->GetPheromoneType()) +=
                    this->GetDistance(pheromone, grid_width * GridCellSize, grid_height * GridCellSize) * pheromone->GetSize()
                    * SETTINGS.physical_constraints.pheromone_detection_sensitivity;
        }
      }
    }

    return pheromone_densities;
}

void PheromoneSystem::ProcessPheromoneDetection(
        std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
        double GridCellSize){
    pheromone_densities_ = GetPheromoneDensities(grid, GridCellSize);
}

std::vector<std::shared_ptr<Pheromone>> PheromoneSystem::EmitPheromones(double deltaTime){
    std::vector<std::shared_ptr<Pheromone>> pheromones;
    for (int type = 0; type < 16; type++){
        if (pheromone_emissions_.at(type) < 0){
            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            std::uniform_real_distribution<> dis(0, 1);
            std::normal_distribution<> nor(0, 0.5);
            if (dis(gen) < pheromone_emissions_.at(type) * size_
                    * SETTINGS.physical_constraints.d_pheromone_emission * deltaTime){
                double x_coord = x_coord_ + nor(gen) * size_;
                double y_coord = y_coord_ + nor(gen) * size_;
                double size = std::sqrt(size);
                pheromones.push_back(std::make_shared<Pheromone>(Pheromone(type, x_coord, y_coord, size)));
            }
        }
    }
    return pheromones;
}
