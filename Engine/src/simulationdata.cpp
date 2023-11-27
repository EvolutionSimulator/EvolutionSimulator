#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

void SimulationData::addEntity(const Entity& entity)
{
    entities_.push_back(entity);
}

void SimulationData::removeEntity(const Entity& entity)
{
    // Use std::remove_if with a lambda function as the predicate
    entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
                                   [&entity](const Entity& e) { return &e == &entity; }),
                    entities_.end());
}


void SimulationData::ModifyAllEntities(double delta_x, double delta_y)
{
    // Iterate through the vector and modify each entity
    std::pair<double, double> coordinates;
    for (Entity& entity : entities_) {
        coordinates = entity.GetCoordinates();
        coordinates.first = coordinates.first + delta_x;
        coordinates.second = coordinates.second + delta_y;
        entity.SetCoordinates(coordinates.first, coordinates.second);
    }
}

void SimulationData::InitializeEntities() {
    // Retrieve information from the environment
    double world_width = environment_.kMapWidth;
    double world_height = environment_.kMapHeight;
    double creature_density = environment_.kCreatureDensity;

    // Clear existing entities
    entities_.clear();

    // Initialize entities randomly on the map
    for (double x = 0; x < world_width; x += 2.0) {
        for (double y = 0; y < world_height; y += 2.0) {
            if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
                Entity newEntity;
                newEntity.RandomInitialization(world_width, world_height);
                entities_.emplace_back(newEntity);
            }
        }
    }
}
