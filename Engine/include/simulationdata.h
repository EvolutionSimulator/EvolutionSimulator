#pragma once
#include "entity.h"
#include <vector>

struct SimulationData {
public:

    SimulationData() : entities_() {
    };

    // Function to add an Entity to the vector
    void addEntity(const Entity& entity);

    // Function to remove an Entity from the vector
    void removeEntity(const Entity& entity);

    // Function to modify all entities, add delta_x and delta_y to the coordinates
    void modifyAllEntities(double delta_x, double delta_y);

    std::vector<Entity> entities_;

};
