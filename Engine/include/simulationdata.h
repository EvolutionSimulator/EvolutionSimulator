#pragma once
#include "creature.h"
#include "environment.h"
#include "food.h"
#include <vector>
#include <queue>
#include <unordered_map>

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
    void CheckCollisions();
    void ModifyAllCreatures(double delta_x, double delta_y);
    void UpdateAllCreatures(double deltaTime);
    void ReproduceCreatures();

    std::vector<Creature> creatures_;
    std::vector<Food> food_entities_;
    std::queue<Creature> fit_reproduce_;
    Environment environment_;
    std::unordered_map<int, std::unordered_map<int, std::vector<Creature>>> creature_grid_;
    std::unordered_map<int, std::unordered_map<int, std::vector<Food>>> food_grid_;

};
