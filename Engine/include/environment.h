#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>

class Environment {
public:
    // Map size
    static constexpr double kMapWidth = 1000.0;
    static constexpr double kMapHeight = 300.0;

    // Member variable for food density

    static constexpr double kCreatureDensity = 0.001; // Constant for creature density

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
    void SetFoodDensity(double density) { kFoodDensity = density;}
    double GetFoodDensity() const {
        return kFoodDensity;
    }
private:
    double kFoodDensity = 0.001;
};


#endif // ENVIRONMENT_H
