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
    double kFoodDensity = 0.001;
    static constexpr int kMaxFoodSize = 10;

    // Length of side of grid cell
    static constexpr double kGridCellSize = 10.0;

    // Tolerance of collisions
    static constexpr double kTolerance = 1e-3;

    // Constructor
    Environment();

    // Getter and setter for food density
    void SetFoodDensity(double density) { kFoodDensity = density;}
    double GetFoodDensity() const {
        return kFoodDensity;
    }};

#endif // ENVIRONMENT_H