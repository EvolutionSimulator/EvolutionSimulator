#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "food.h"
#include <cmath>
#include "collisions.h"
#include <cassert>
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

myEnvironment::Environment SimulationData::GetEnvironment()
{
    return environment_;
}

void SimulationData::SetEnvironment(myEnvironment::Environment& environment)
{
    environment_ = environment;
}

std::vector<std::vector<std::vector<Entity*> > > SimulationData::GetGrid()
{
    return grid_;
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
        creature.SetCoordinates(coordinates.first, coordinates.second, settings::environment::kMapWidth, settings::environment::kMapHeight);
    }
}
/*!
 * Update all creatures position and orientation by a deltaTime interval
 */
void SimulationData::UpdateAllCreatures(double deltaTime)
{
    for (Creature& creature: creatures_) {
        creature.Update(deltaTime, settings::environment::kMapWidth, settings::environment::kMapHeight, grid_, settings::environment::kGridCellSize);
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
    double max_number = environment_.GetFoodDensity() * settings::environment::kMapHeight * settings::environment::kMapWidth / 100;
    while (size < max_number){
        Food new_food = Food();
        new_food.RandomInitialization(settings::environment::kMapWidth,
                                      settings::environment::kMapHeight,
                                      settings::environment::kMaxFoodSize,
                                      settings::environment::kMinCreatureSize);
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


/*!
 * Initializes creatures randomly on the map, mutating their genome 30 times
 */
void SimulationData::InitializeCreatures() {
    // Retrieve information from the environment
    double world_width = settings::environment::kMapWidth;
    double world_height = settings::environment::kMapHeight;
    double creature_density = settings::environment::kCreatureDensity;
    double max_creature_size = settings::environment::kMaxCreatureSize;
    double min_creature_size = settings::environment::kMinCreatureSize;

    creatures_.clear();

    for (double x = 0; x < world_width; x += 2.0) {
        for (double y = 0; y < world_height; y += 2.0) {
            if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
                neat::Genome genome(settings::environment::kInputNeurons, settings::environment::kOutputNeurons);
                for (int i = 0; i < 30; i++){genome.Mutate();}
                Creature new_creature(genome);
                new_creature.RandomInitialization(world_width, world_height, max_creature_size, min_creature_size);
                creatures_.emplace_back(new_creature);
            }
        }
    }
}

/*!
 * Initializes food randomly on the map
 */

void SimulationData::InitializeFood() {
    double kFoodDensity = environment_.GetFoodDensity();
    food_entities_.clear();

    // Populate the vector with food entities based on the current food density
    for (double x = 0; x < settings::environment::kMapWidth; x += 10.0) {
        for (double y = 0; y < settings::environment::kMapHeight; y += 10.0) {
            if (std::rand() / (RAND_MAX + 1.0) < kFoodDensity) {
                food_entities_.emplace_back(Food(x,y));
            }
        }
    }
}

/*!
 * Initializes the empty grid to place entities in and places them in the right square
 */
void SimulationData::InitializeGrid() {

    // Number of grid cells
    int num_cells_x = static_cast<int>(std::ceil(static_cast<double>(settings::environment::kMapWidth) / settings::environment::kGridCellSize)) + 1;
    int num_cells_y = static_cast<int>(std::ceil(static_cast<double>(settings::environment::kMapHeight) / settings::environment::kGridCellSize)) + 1;
    // Resize the grid to the specified dimensions
    grid_.assign(num_cells_x, std::vector<std::vector<Entity*> >(num_cells_y));

    UpdateGrid();
}

void SimulationData::ClearGrid() {
    for (auto& row : grid_) {
        for (auto& cell : row) {
            cell.clear();
        }
    }
}

/*!
 * Deletes dead entities, and places remaining ones in grid
 */
template <typename EntityType>
void UpdateGridTemplate(std::vector<EntityType>& entities, std::vector<std::vector<std::vector<Entity*> > >& entityGrid, double cellSize) {

    entities.erase(std::remove_if(entities.begin(), entities.end(), [](const EntityType& entity) {
                       return entity.GetState() != Entity::Alive;
                   }), entities.end());

    for (EntityType& entity : entities) {
        std::pair<double, double> coordinates = entity.GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(&entity);
    }
}


void SimulationData::UpdateGrid() {
    ClearGrid();
    UpdateGridTemplate<Creature>(creatures_, grid_, settings::environment::kGridCellSize);
    UpdateGridTemplate<Food>(food_entities_, grid_, settings::environment::kGridCellSize);
}

std::vector<std::pair<int, int>> GetNeighbours(const int& num_rows, const int& num_cols, const std::pair<int, int>& center, const int& layer_number){
    int x_start = center.first-layer_number;
    int x_end = center.first+layer_number+1;
    int y_start = center.second-layer_number;
    int y_end = center.second+layer_number+1;

    if (x_start<0) x_start = 0;
    if (x_end>num_rows) x_end = num_rows;
    if (y_start<0) y_start = 0;
    if (y_end>num_cols) y_end = num_cols;

    std::vector<std::pair<int, int>> neighbours;
    for (int x = x_start; x<x_end; x++){
        for (int y = y_start; y<y_end; y++){
            neighbours.push_back(std::make_pair(x, y));
        }
    }
    return neighbours;
}

void SimulationData::CheckCollisions() {
    double tolerance = settings::environment::kTolerance;
    int num_rows = static_cast<int>(std::ceil(static_cast<double>(settings::environment::kMapWidth) / settings::environment::kGridCellSize))+1;
    int num_cols = static_cast<int>(std::ceil(static_cast<double>(settings::environment::kMapHeight) / settings::environment::kGridCellSize))+1;
    for (int row = 0; row<num_rows; row++){
        for (int col = 0; col<num_cols; col++){
            for (Entity* entity1: grid_[row][col]){
                const int layer_number = 2*ceil((entity1->GetSize()/settings::environment::kGridCellSize));
                std::vector<std::pair<int, int> > neighbours = GetNeighbours(num_rows, num_cols, {row, col}, layer_number);
                for (const std::pair<int, int> neighbour: neighbours){
                    for (Entity* entity2: grid_[neighbour.first][neighbour.second]){
                        if (entity1->CheckCollisionWithEntity(tolerance, *entity2)) {
                            if (entity1 != entity2) {
                                entity1->OnCollision(*entity2, settings::environment::kMapWidth, settings::environment::kMapHeight);
                            }
                        }
                    }
                }
            }
        }
    }
}
