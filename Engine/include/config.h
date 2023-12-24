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
}  // namespace neat

namespace environment {
constexpr double kMapWidth = 1000.0;
constexpr double kMapHeight = 450.0;
constexpr double kCreatureDensity = 0.0005;
constexpr int kMaxFoodSize = 15;
constexpr int kMaxCreatureSize = 15;
constexpr double kTolerance = 1e-3;
constexpr double kDefaultFoodDensity = 0.005;
constexpr double kEnergyToHealth = 70.0;
constexpr double kHealthToEnergy = 10.0;
constexpr double kDefaultCreatureDensity = 0.0001;
constexpr double kPlantNutritionalValue = 2.0;
constexpr double kMeatNutritionalValue = 3.0;
constexpr double kRotFactor = 1.0;
constexpr double kGridCellSize = 50.0;
constexpr int kMinCreatureSize = 2;
constexpr double kReproductionThreshold = 0.90;
constexpr double kReproductionCooldown = 10;
constexpr int kInputNeurons = 6;
constexpr int kOutputNeurons = 3;
constexpr double kMaxNutritionalValue = 5;
constexpr double kDefaultLifespan = 30;
constexpr double kPhotosynthesisFactor = 0.1;
constexpr double kVisionRadius = 200;
constexpr double kVisionAngle = M_PI / 3;
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
constexpr double kMaxEnergyDensity = 1.0;
constexpr double kMinEnergyLoss = 0.1;
constexpr double kDEnergyDensity = 0.5;
constexpr double kDEnergyLoss = 0.5;
constexpr double kDStrafingDifficulty = 0.5;
constexpr double kDMaxSize = 10;
constexpr double kDBabySize = 2;
constexpr double kDMaxForce = 10;
constexpr double kDGrowthFactor = 5;

} //namespace physical_constraints
}  // namespace settings

#endif  // CONFIG_H
