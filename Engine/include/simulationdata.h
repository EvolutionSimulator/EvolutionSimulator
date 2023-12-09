#pragma once
#include "creature.h"
#include "environment.h"
#include "food.h"
#include <vector>
#include <unordered_map>
#include <queue>

struct SimulationData {
public:

    SimulationData() : creatures_(), environment_(){
        InitializeFood();
        InitializeCreatures();
        InitializeGrid();
    };

    void InitializeCreatures();
    void InitializeFood();
    void InitializeGrid();

    void AddCreature(const Creature& entity);
    void RemoveCreature(const Creature& entity);

    void GenerateMoreFood();
    void UpdateGrid();

    void CheckFoodCollisions();
    void CheckCreatureCollisions();

    void ModifyAllCreatures(double delta_x, double delta_y);
    void UpdateAllCreatures(double deltaTime);

    void ReproduceCreatures();

    std::vector<Creature> creatures_;
    std::vector<Food> food_entities_;
    std::queue<Creature> reproduce_;
    Environment environment_;
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> creature_grid_;
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> food_grid_;

};
