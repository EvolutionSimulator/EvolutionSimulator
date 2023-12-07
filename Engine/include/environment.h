#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include <iostream>

class Environment {
public:
    // Map size
    static constexpr double kMapWidth = 1000.0;
    static constexpr double kMapHeight = 300.0;

    static constexpr double kCreatureDensity = 0.001; // Constant for creature density
    double kFoodDensity; // Variable for creature density


    // Food and creature density for random initialization
    static constexpr int kMaxFoodSize = 30;
    static constexpr int kMaxCreatureSize = 30;

    // Length of side of grid cell
    static constexpr double kGridCellSize = 10.0;

    // Tolerance of collisions
    static constexpr double kTolerance = 1e-3;

    // Constructor
    Environment();

    // Getter and setter for food density
    void SetFoodDensity(double density) {
        kFoodDensity = density;
    }
    double GetFoodDensity() const {
        return kFoodDensity;
    }
};


#endif // ENVIRONMENT_H
