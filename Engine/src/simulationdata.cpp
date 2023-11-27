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


void SimulationData::modifyAllEntities(double delta_x, double delta_y)
{
    // Iterate through the vector and modify each entity
    std::pair<double, double> coordinates;
    for (Entity& entity : entities_) {
        coordinates = entity.getCoordinates();
        coordinates.first = coordinates.first + delta_x;
        coordinates.second = coordinates.second + delta_y;
        entity.setCoordinates(coordinates);
    }
}
