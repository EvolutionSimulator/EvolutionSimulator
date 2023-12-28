#ifndef CONFIG_H
#define CONFIG_H
#include <cmath>

namespace settings {

namespace neat {
constexpr double kWeightMutationRate = 0.2;
constexpr double kStandardDeviationWeight = 0.1;
constexpr double kMaxWeight = 1.0;
constexpr double kMinWeight = 0.0;
constexpr double kAddNeuronMutationRate = 0.05;
constexpr double kAddLinkMutationRate = 0.25;
constexpr double kRemoveNeuronMutationRate = 0.02;
constexpr double kRemoveLinkMutationRate = 0.15;
constexpr double kChangeWeightMutationRate = 0.6;
constexpr double kBiasMutationRate = 0.2;
constexpr double kMaxBias = 0.1;
constexpr double kMinBias = 0.0;
constexpr double kChangeBiasMutationRate = 0.6;
constexpr double kAdjustmentProbability = 0.8;
constexpr double kActivationFunctionMutationRate = 0.02;
}  // namespace neat

namespace compatibility {
constexpr double kWeightSharedNeurons = 0.2;
constexpr double kWeightSharedLinks = 0.3;
constexpr double kAverageWeightSharedLinks = 0.5;
constexpr double kColorCompatibility = 0.1;
constexpr double kMutablesCompatibility = 0.5;
constexpr double kCompatibilityThreshold = 1;
}

namespace environment {
constexpr double kDMapWidth = 1900.0; //not used currently
constexpr double kDMapHeight = 880.0; //not used currently
constexpr double kCreatureDensity = 0.0005;
constexpr int kMaxFoodSize = 15; // Influences vision
constexpr int kMaxCreatureSize = 15;
constexpr double kTolerance = 1e-3;
constexpr double kDefaultFoodDensity = 0.005;
constexpr double kEnergyToHealth = 70.0;
constexpr double kHealthToEnergy = 10.0;
constexpr double kDefaultCreatureDensity = 0.0001;
constexpr double kPlantNutritionalValue = 2.0;
constexpr double kMeatNutritionalValue = 3.0;
constexpr double kPlantProportion = 0.5; /* PlantProportion + MeatProportion = 1 */
constexpr double kRotFactor = 1.0;
constexpr double kGridCellSize = 50.0;
constexpr int kMinCreatureSize = 2;
constexpr double kReproductionThreshold = 0.80;
constexpr double kReproductionCooldown = 10;
constexpr int kInputNeurons = 7;
constexpr int kOutputNeurons = 4;
constexpr double kMaxNutritionalValue = 5;
constexpr double kDefaultLifespan = 30;
constexpr double kPhotosynthesisFactor = 0.1;
constexpr double kFrictionalCoefficient =
    0.5;  // Needs to be less than 1
}  // namespace environment

namespace engine {
constexpr double kFixedUpdateInterval = 0.05;
constexpr double EPS = 1e-7;
constexpr size_t kMaxCellsToFindFood = 30;
}  // namespace engine

namespace physical_constraints {
constexpr double kMutationRate = 0.2;
constexpr double kMaxEnergyDensity = 10.0;
constexpr double kMinEnergyLoss = 0.1;
constexpr double kDEnergyDensity = 5.0;
constexpr double kDEnergyLoss = 0.5;
constexpr double kDIntegrity = 3.0;
constexpr double kDStrafingDifficulty = 0.5;
constexpr double kDMaxSize = 10;
constexpr double kDBabySize = 2;
constexpr double kDMaxForce = 10;
constexpr double kDGrowthFactor = 10;
constexpr double kDVisionFactor = 200;
constexpr double kVisionRadius = 200;
constexpr double kVisionAngle = M_PI / 3;
constexpr double kVisionARratio = 200 * M_PI / 3;

} //namespace physical_constraints
}  // namespace settings

#endif  // CONFIG_H
