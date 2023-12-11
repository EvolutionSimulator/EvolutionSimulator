#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "food.h"
#include <cmath>
#include "collisions.h"
#include "config.h"

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
void SimulationData::UpdateAllCreatures(double deltaTime)
{
    for (Creature& creature: creatures_) {
        creature.Update(deltaTime, environment_.kMapWidth, environment_.kMapHeight);
        if (creature.Fit()){
            reproduce_.push(creature);
        }
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

/*!
 * Takes the creatures from the reproduce queue in pairs and initializaes randomly a crossedover version
 */
void SimulationData::ReproduceCreatures(){
    while (reproduce_.size() > 2){
        Creature creature1 = reproduce_.front();
        reproduce_.pop();
        Creature creature2 = reproduce_.front();
        reproduce_.pop();
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
                neat::Genome genome(cfg::input_neurons, cfg::output_neurons);
                genome.Mutate();
                Creature new_creature(genome);
                new_creature.RandomInitialization(world_width, world_height, max_creature_size);
                creatures_.emplace_back(new_creature);
            }
        }
    }
}

// Function to initialize the environment
void SimulationData::InitializeFood() {
    double kFoodDensity = environment_.GetFoodDensity(); // Use the getter here

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
            grid_[i][j] = std::vector<Entity*>();
        }
    }

    UpdateGrid();
}

void SimulationData::ClearGrid() {
    for (auto& row : grid_) {
        for (auto& cell : row.second) {
            cell.second.clear();
        }
    }
}


template <typename EntityType>
void UpdateGridTemplate(std::vector<EntityType>& entities, std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>>& entityGrid, double cellSize) {

    // Remove dead entities from the entities vector and update the grid for alive entities
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](const EntityType& entity) {
                       return entity.GetState() != Entity::Alive;
                   }), entities.end());

    // Update the grid based on entity positions and store entity indices
    for (EntityType& entity : entities) {
        // Update the grid for alive entities
        std::pair<double, double> coordinates = entity.GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(&entity);
    }
}

void SimulationData::UpdateGrid() {
    ClearGrid();
    UpdateGridTemplate<Creature>(creatures_, grid_, environment_.kGridCellSize);
    UpdateGridTemplate<Food>(food_entities_, grid_, environment_.kGridCellSize);
}

std::vector<std::pair<int, int>> GetNeighbours(const int& dimension_x, const int& dimension_y, const std::pair<int, int>& center, const int& layer_number){
    int x_start = center.first-layer_number;
    int x_end = center.first+layer_number+1;
    int y_start = center.second-layer_number;
    int y_end = center.second+layer_number+1;

    if (x_start<0) x_start = 0;
    if (x_end>dimension_x) x_end = dimension_x;
    if (y_start<0) y_start = 0;
    if (y_end>dimension_x) y_end = dimension_y;

    std::vector<std::pair<int, int>> neighbours;
    for (int x = x_start; x<x_end; x++){
        for (int y = y_start; y<y_end; y++){
            neighbours.push_back(std::make_pair(x, y));
        }
    }
    return neighbours;
}

void SimulationData::CheckCollisions() {
    double tolerance = environment_.kTolerance;
    const int numRows = grid_.size(); // Number of rows
    const int numCols = (numRows > 0) ? grid_[0].size() : 0;
    for (int row = 0; row<numRows; row++){
        for (int col = 0; col<numCols; col++){
            //check the proportion and check all neighbours
            for (Entity* entity1: grid_[row][col]){
                const int layer_number = ceil(2*entity1->GetSize());
                std::vector<std::pair<int, int>> neighbours = GetNeighbours(numCols, numRows, {row, col}, layer_number);
                for (const std::pair<int, int> neighbour: neighbours){
                    for (Entity* entity2: grid_[neighbour.first][neighbour.second]){
                        if (entity1->CheckCollisionWithEntity(tolerance, *entity2)) {
                            // Check if entity1 is a Creature and entity2 is a Food
                            if (Creature* creature = dynamic_cast<Creature*>(entity1)) {
                                if (Food* food = dynamic_cast<Food*>(entity2)) {
                                    creature->OnCollision(*food);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
