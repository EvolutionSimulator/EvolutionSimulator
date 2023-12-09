#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include <iostream>

class Environment {
public:
    // Map size
    static constexpr double kMapWidth = 1100.0;
    static constexpr double kMapHeight = 450.0;

    // Variable for creature and food density
    double kFoodDensity;
    double kCreatureDensity = 0.001;

    // Food and creature density for random initialization
    static constexpr int kMaxFoodSize = 30;
    static constexpr int kMaxCreatureSize = 30;

    // Length of side of grid cell
    static constexpr double kGridCellSize = 10.0;

    // Tolerance of collisions
    static constexpr double kTolerance = 1e-3;

    // Energy and Health conversion thresholds
    static constexpr double energyToHealth = 70.0;
    static constexpr double healthToEnergy = 10.0;

    // Constructor
    Environment();

    // Getter and setter for food density
    void SetFoodDensity(double density) {
        kFoodDensity = density;
    }
    double GetFoodDensity() const {
        return kFoodDensity;
    }

    // Getter and setter for creature density
    void SetCreatureDensity(double density) { kCreatureDensity = density;}
    double GetCreatureDensity() const {
        return kCreatureDensity;
    }
};


#endif // ENVIRONMENT_H
