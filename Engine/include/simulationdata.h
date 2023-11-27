#pragma once
#include "entity.h"
#include <vector>

struct SimulationData {
public:

    SimulationData() : entities_() {
    };

    void addEntity(const Entity& entity);
    void removeEntity(const Entity& entity);

    // Test function (DO NOT USE)
    void ModifyAllEntities(double delta_x, double delta_y);

    std::vector<Entity> entities_;

};
