#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>

class Environment {
public:
    // Map size
    static constexpr double kMapWidth = 1100.0;
    static constexpr double kMapHeight = 450.0;

    // Member variable for food density

    static constexpr double kCreatureDensity = 0.0005; // Constant for creature density

    // Food and creature density for random initialization
    static constexpr int kMaxFoodSize = 15;
    static constexpr int kMaxCreatureSize = 25;

    // Length of side of grid cell
    static constexpr double kGridCellSize = 50.0;

    // Tolerance of collisions
    static constexpr double kTolerance = 1e-3;

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
