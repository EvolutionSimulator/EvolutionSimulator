#include "simulationdata.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

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


void SimulationData::ModifyAllCreatures(double delta_x, double delta_y)
{
    // Iterate through the vector and modify each entity
    std::pair<double, double> coordinates;
    for (Creature& creature : creatures_) {
        coordinates = creature.GetCoordinates();
        coordinates.first = coordinates.first + delta_x;
        coordinates.second = coordinates.second + delta_y;
        creature.SetCoordinates(coordinates.first, coordinates.second);
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
                Creature new_creature;
                new_creature.RandomInitialization(world_width, world_height);
                creatures_.emplace_back(new_creature);
            }
        }
    }
}
// Function to initialize the environment
void SimulationData::InitializeFood() {
    // Populate the vector with food entities based on food density
    for (double x = 0; x < environment_.kMapWidth; x += 10.0) {
        for (double y = 0; y < environment_.kMapHeight; y += 10.0) {
            if (std::rand() / (RAND_MAX + 1.0) < environment_.kFoodDensity) {

                food_entities_.emplace_back(Food(x,y));
            }
        }
    }
}
