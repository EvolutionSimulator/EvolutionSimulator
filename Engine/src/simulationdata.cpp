#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "food.h"
#include <cmath>
#include "collisions.h"

void SimulationData::AddCreature(const Creature& creature)
{
    creatures_.push_back(creature);
}

void SimulationData::RemoveCreature(const Creature& creature)
{
    // Use std::remove_if with a lambda function as the predicate
    creatures_.erase(std::remove_if(creatures_.begin(), creatures_.end(),
                                   [&creature](const Creature& e) { return &e == &creature; }),
                     creatures_.end());
}

/*!
 * Iterate through the vector and modify each entity
 */
void SimulationData::ModifyAllCreatures(double delta_x, double delta_y)
{
    std::pair<double, double> coordinates;
    for (Creature& creature : creatures_) {
        coordinates = creature.GetCoordinates();
        coordinates.first = coordinates.first + delta_x;
        coordinates.second = coordinates.second + delta_y;
        creature.SetCoordinates(coordinates.first, coordinates.second, environment_.kMapWidth, environment_.kMapHeight);
    }
}
/*!
 * Update all creatures position and orientation by a deltaTime interval
 */
void SimulationData::MoveAllCreatures(double deltaTime)
{
    for (Creature& creature: creatures_) {
        creature.Rotate(deltaTime);
        creature.Move(deltaTime, environment_.kMapWidth, environment_.kMapHeight);
    }
}

/*!
 * If the food density is less than the established in the environment it creates new food entities until that threshold is reached
 */
void SimulationData::GenerateMoreFood(){
    double size = food_entities_.size();
    double max_number = environment_.GetFoodDensity() * environment_.kMapHeight * environment_.kMapWidth;
    while (size < max_number){
        Food new_food = Food();
        new_food.RandomInitialization(environment_.kMapHeight, environment_.kMapWidth, environment_.kMaxCreatureSize);
        food_entities_.emplace_back(new_food);
        size++;
    }
}

void SimulationData::InitializeCreatures() {
    // Retrieve information from the environment
    double world_width = environment_.kMapWidth;
    double world_height = environment_.kMapHeight;
    double creature_density = environment_.kCreatureDensity;
    double max_creature_size = environment_.kMaxCreatureSize;

    // Clear existing entities
    creatures_.clear();

    // Initialize entities randomly on the map
    for (double x = 0; x < world_width; x += 2.0) {
        for (double y = 0; y < world_height; y += 2.0) {
            if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
                Creature new_creature;
                new_creature.RandomInitialization(world_width, world_height, max_creature_size);
                creatures_.emplace_back(new_creature);
            }
        }
    }
}

// Function to initialize the environment
void SimulationData::InitializeFood() {
    double kFoodDensity = environment_.GetFoodDensity(); // Use the getter here

    std::cout<<"Density in Simulation data:"<<kFoodDensity<<std::endl;

    // Clear existing food entities before repopulating
    food_entities_.clear();

    // Populate the vector with food entities based on the current food density
    for (double x = 0; x < environment_.kMapWidth; x += 10.0) {
        for (double y = 0; y < environment_.kMapHeight; y += 10.0) {
            if (std::rand() / (RAND_MAX + 1.0) < kFoodDensity) {
                food_entities_.emplace_back(Food(x,y));
            }
        }
    }
}

void SimulationData::InitializeGrid() {

    // Number of grid cells
    int num_cells_x = environment_.kMapWidth / environment_.kGridCellSize;
    int num_cells_y = environment_.kMapHeight / environment_.kGridCellSize;

    // Initialize the grid
    for (int i = 0; i < num_cells_x; ++i) {
        for (int j = 0; j < num_cells_y; ++j) {
            creature_grid_[i][j] = std::vector<int>();
            food_grid_[i][j] = std::vector<int>();
        }
    }

    UpdateGrid();
}

template <typename EntityType>
void UpdateGridTemplate(std::vector<EntityType>& entities, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& entityGrid, double cellSize) {
    // Clear the grid
    for (auto& row : entityGrid) {
        for (auto& cell : row.second) {
            cell.second.clear();
        }
    }

    // Remove dead entities from the entities vector and update the grid for alive entities
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](const EntityType& entity) {
                       return entity.GetState() != Entity::Alive;
                   }), entities.end());

    // Update the grid based on entity positions and store entity indices
    for (size_t i = 0; i < entities.size(); ++i) {
        const EntityType& entity = entities[i];
        // Update the grid for alive entities
        std::pair<double, double> coordinates = entity.GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);
        entityGrid[gridX][gridY].push_back(static_cast<int>(i));
    }
}

void SimulationData::UpdateGrid() {
    UpdateGridTemplate(creatures_, creature_grid_, environment_.kGridCellSize);
    UpdateGridTemplate(food_entities_, food_grid_, environment_.kGridCellSize);
}


template<typename EntityType1, typename EntityType2>
void CheckCollisionsTemplate(
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& entityGrid1,
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& entityGrid2,
    std::vector<EntityType1>& entity_vector_1,
    std::vector<EntityType2>& entity_vector_2,
    const Environment& environment
    ) {
    double tolerance = environment.kTolerance;

    for (auto& row : entityGrid1) {
        for (auto& cell : row.second) {
            std::vector<int>& entities1 = cell.second;

            // Check if there are entities in the corresponding cell in entityGrid2
            auto it2 = entityGrid2.find(row.first);
            if (it2 != entityGrid2.end()) {
                auto& row2 = *it2;
                auto itCell2 = row2.second.find(cell.first);
                if (itCell2 != row2.second.end()) {
                    std::vector<int>& entities2 = itCell2->second;

                    // Now, check for collisions only between entities1 and entities2
                    for (auto& entity1_index : entities1) {
                        for (auto& entity2_index : entities2) {
                            EntityType1& entity1 = entity_vector_1[entity1_index];
                            EntityType2& entity2 = entity_vector_2[entity2_index];
                            if (entity1.CheckCollisionWithEntity(tolerance, entity2)) {
                                // Handle the collision as needed<
                                entity1.OnCollision(entity2);
                            }
                        }
                    }
                }
            }
        }
    }
}

void SimulationData::CheckFoodCollisions() {
    CheckCollisionsTemplate<Creature,Food>(creature_grid_, food_grid_, creatures_, food_entities_, environment_);
}
