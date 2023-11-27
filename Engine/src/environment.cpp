// Environment.cpp

#include "environment.h"
#include <random>

// Constructor implementation
Environment::Environment() {
    // Other constructor logic, if needed
}

// Function to initialize the environment
void Environment::InitializeEnvironment() {
    // Populate the vector with food entities based on food density
    for (double x = 0; x < kMapWidth; x += 10.0) {
        for (double y = 0; y < kMapHeight; y += 10.0) {
            if (std::rand() / (RAND_MAX + 1.0) < kFoodDensity) {

                food_entities_.emplace_back(Food(x,y));
            }
        }
    }
}
