#pragma once
#include "entity.h"
#include "environment.h"
#include <vector>

struct SimulationData {
public:

    SimulationData() : entities_(), environment_(){
        environment_.InitializeEnvironment();
        InitializeEntities();
    };

    void InitializeEntities();

    void addEntity(const Entity& entity);
    void removeEntity(const Entity& entity);

    // Test function (DO NOT USE)
    void ModifyAllEntities(double delta_x, double delta_y);

    std::vector<Entity> entities_;
    Environment environment_;

};
