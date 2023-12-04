#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "food.h"
#include <vector>

class Environment {
public:
    // Constants for map size
    static constexpr double kMapWidth = 1000.0;
    static constexpr double kMapHeight = 300.0;

    // Member variable for food density
    double foodDensity; // Non-const, allows runtime changes

    static constexpr double kCreatureDensity = 0.001; // Constant for creature density

    // Constructor
    Environment();

    // Getter and setter for food density
    void SetFoodDensity(double density) { foodDensity = density; printf("setting density to %f\n", density);}
    double GetFoodDensity() const {
        printf("Getting density: %f\n", foodDensity);
        return foodDensity;
    }};

#endif // ENVIRONMENT_H
