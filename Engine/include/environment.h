// Environment.h

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "food.h"
#include <vector>

class Environment {
public:
    // Constants for map size
    static constexpr double kMapWidth = 1000.0;
    static constexpr double kMapHeight = 300.0;

    // Constant for food density
    static constexpr double kFoodDensity = 0.001;
    static constexpr int kMaxFoodSize = 10;
    static constexpr double kCreatureDensity = 0.001;

    // Constructor
    Environment();
};

#endif // ENVIRONMENT_H
