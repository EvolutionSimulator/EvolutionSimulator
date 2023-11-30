#pragma once
#include "creature.h"
#include "environment.h"
#include <vector>

struct SimulationData {
public:

    SimulationData() : creatures_(), environment_(){
        InitializeFood();
        InitializeCreatures();
    };

    void InitializeCreatures();
    void InitializeFood();

    void AddCreature(const Creature& entity);
    void RemoveCreature(const Creature& entity);

    void GenerateMoreFood();
    // Test function (DO NOT USE)
    void ModifyAllCreatures(double delta_x, double delta_y);
    void MoveAllCreatures(double deltaTime);

    std::vector<Creature> creatures_;
    std::vector<Food> food_entities_;
    Environment environment_;

};
