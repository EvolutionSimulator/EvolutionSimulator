#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "food.h"
#include <cmath>
#include "collisions.h"
#include "neat/neat-genome.h"

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
        creature.SetCoordinates(coordinates.first, coordinates.second);
    }
}
/*!
 * Update all creatures position and orientation by a deltaTime interval
 */
void SimulationData::UpdateAllCreatures(double deltaTime)
{
    for (Creature& creature: creatures_) {
        creature.Update(deltaTime);
        if (creature.Fit()){
            fit_reproduce_.push(creature);
        }
    }
}

/*!
 * Takes two creatures from fit_reproduce and makes a children out of them, takes the creature with the most energy as the dominant genome
 */
void SimulationData::ReproduceCreatures(){
    while (fit_reproduce_.size() > 2){
        Creature creature1 = fit_reproduce_.front();
        fit_reproduce_.pop();
        Creature creature2 = fit_reproduce_.front();
        fit_reproduce_.pop();
        double energy1 = creature1.GetEnergy();
        double energy2 = creature2.GetEnergy();
        if (energy1 > energy2){
            neat::Genome new_genome = neat::Crossover(creature1.GetGenome(), creature2.GetGenome());
            AddCreature(Creature(new_genome));
        } else {
            neat::Genome new_genome = neat::Crossover(creature2.GetGenome(), creature1.GetGenome());
            AddCreature(Creature(new_genome));
        }
    }
}
/*!
 * If the food density is less than the established in the environment it creates new food entities until that threshold is reached
 */
void SimulationData::GenerateMoreFood(){
    double size = food_entities_.size();
    double max_number = environment_.kFoodDensity * environment_.kMapHeight * environment_.kMapWidth;
    while (size < max_number){
        Food new_food = Food();
        new_food.RandomInitialization(environment_.kMapHeight, environment_.kMapWidth);
        food_entities_.emplace_back(new_food);
        size++;
    }
}

void SimulationData::InitializeCreatures() {
    // Retrieve information from the environment
    double world_width = environment_.kMapWidth;
    double world_height = environment_.kMapHeight;
    double creature_density = environment_.kCreatureDensity;

    // Clear existing entities
    creatures_.clear();

    // Initialize entities randomly on the map
    for (double x = 0; x < world_width; x += 2.0) {
        for (double y = 0; y < world_height; y += 2.0) {
            if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
                neat::Genome genome(7,3);
                genome.Mutate();
                Creature new_creature(genome);
                new_creature.RandomInitialization(world_width, world_height);
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
            creature_grid_[i][j] = std::vector<Creature>();
            food_grid_[i][j] = std::vector<Food>();
        }
    }

    UpdateGrid();
}

template <typename EntityType>
void UpdateGridTemplate(const std::vector<EntityType>& entities, std::unordered_map<int, std::unordered_map<int, std::vector<EntityType>>>& entityGrid, double cellSize) {
    // Clear the grid
    for (auto& row : entityGrid) {
        for (auto& cell : row.second) {
            cell.second.clear();
        }
    }

    // Update the grid based on entity positions
    std::pair<double, double> coordinates;
    int gridX, gridY;

    for (const auto& entity : entities) {
        coordinates = entity.GetCoordinates();
        gridX = static_cast<int>(coordinates.first / cellSize);
        gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(entity);
    }
}

void SimulationData::UpdateGrid() {
    UpdateGridTemplate(creatures_, creature_grid_, environment_.kGridCellSize);
    UpdateGridTemplate(food_entities_, food_grid_, environment_.kGridCellSize);
}


template<typename EntityType1, typename EntityType2>
void CheckCollisionsTemplate(
    const std::unordered_map<int, std::unordered_map<int, std::vector<EntityType1>>>& entityGrid1,
    const std::unordered_map<int, std::unordered_map<int, std::vector<EntityType2>>>& entityGrid2,
    const Environment& environment
    ) {
    double tolerance = environment.kTolerance;

    for (const auto& row : entityGrid1) {
        for (const auto& cell : row.second) {
            const std::vector<EntityType1>& entities1 = cell.second;

            // Check if there are entities in the corresponding cell in entityGrid2
            auto it2 = entityGrid2.find(row.first);
            if (it2 != entityGrid2.end()) {
                const auto& row2 = *it2;
                auto itCell2 = row2.second.find(cell.first);
                if (itCell2 != row2.second.end()) {
                    const std::vector<EntityType2>& entities2 = itCell2->second;

                    // Now, check for collisions only between entities1 and entities2
                    for (const auto& entity1 : entities1) {
                        for (const auto& entity2 : entities2) {
                            if (CollisionCircleCircle(tolerance, entity1.GetCoordinates(), entity1.GetSize(), entity2.GetCoordinates(), entity2.GetSize())) {
                                // Handle the collision as needed

                            }
                        }
                    }
                }
            }
        }
    }
}

void SimulationData::CheckCollisions() {
    CheckCollisionsTemplate<Creature,Food>(creature_grid_, food_grid_, environment_);
}
