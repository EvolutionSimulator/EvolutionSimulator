#pragma once
#include "creature.h"
#include "environment.h"
#include "food.h"
#include <vector>
#include <unordered_map>
#include <queue>

struct SimulationData {
public:

  explicit SimulationData(myEnvironment::Environment& env)
        : environment_(env), creatures_(), food_entities_() {
        InitializeFood();
        InitializeCreatures();
        InitializeGrid();
    }

    void InitializeCreatures();
    void InitializeFood();
    void InitializeGrid();

    void AddCreature(const Creature& entity);
    void RemoveCreature(const Creature& entity);

    void GenerateMoreFood();
    void UpdateGrid();
    void ClearGrid();

    void CheckFoodCollisions();
    void CheckCreatureCollisions();
    void CheckCollisions();

    void ModifyAllCreatures(double delta_x, double delta_y);
    void UpdateAllCreatures(double deltaTime);

    myEnvironment::Environment GetEnvironment();
    myEnvironment::Environment SetEnvironment(myEnvironment::Environment& environment);
    std::vector<std::vector<std::vector<Entity*> > > GetGrid();


    void ReproduceCreatures();

    std::vector<Creature> creatures_;
    std::vector<Food> food_entities_;
    std::queue<Creature> reproduce_;

    double world_time_ = 0;
private:
    myEnvironment::Environment& environment_;
    std::vector<std::vector<std::vector<Entity*> > > grid_;

};
