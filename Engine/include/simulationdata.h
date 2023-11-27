#pragma once
#include "entity.h"
#include <vector>

struct SimulationData {
public:

    SimulationData(double world_size) : entities_() {
        world_size_ = world_size;
    };

    void addEntity(const Entity& entity);
    void removeEntity(const Entity& entity);

    // Test function (DO NOT USE)
    void ModifyAllEntities(double delta_x, double delta_y);

    std::vector<Entity> entities_;

private:
    double world_size_;

};
