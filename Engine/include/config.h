// Unused, kept to save the value of the constants

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
constexpr double kModuleActivationMutationRate = 0.01;
constexpr double kModuleDisableMutationRate = 0.01;
}  // namespace neat

namespace compatibility {
constexpr double kWeightSharedNeurons = 0.2;
constexpr double kWeightSharedLinks = 0.3;
constexpr double kAverageWeightSharedLinks = 0.5;
constexpr double kColorCompatibility = 0.1;
constexpr double kMutablesCompatibility = 0.5;
constexpr double kCompatibilityThreshold = 2;
constexpr double kCompatibilityDistance = 100;
}  // namespace compatibility

namespace environment {
constexpr double kDMapWidth = 1900.0;  // not used currently
constexpr double kDMapHeight = 880.0;  // not used currently
constexpr double kCreatureDensity = 0.0005;
constexpr int kMaxFoodSize = 15;  // Influences vision
constexpr int kMaxCreatureSize = 15;
constexpr double kTolerance = 1e-3;
constexpr double kDefaultFoodDensity = 5e-4;
constexpr double kFoodSpawnRate = 1e-4;
constexpr double kEnergyToHealth = 70.0;
constexpr double kHealthToEnergy = 10.0;
constexpr double kDefaultCreatureDensity = 5e-4;
constexpr double kPlantNutritionalValue = 1.0;
constexpr double kMeatNutritionalValue = 2.0;
<<<<<<< HEAD
constexpr double kEggNutritionalValue = 3.0;
constexpr double kEggIncubationTimeMultiplier = 1.0;
constexpr double kPlantProportion =
    0.5; /* PlantProportion + MeatProportion = 1 */
constexpr double kRotFactor = 1.0;
=======
constexpr double kPlantProportion = 0.5; /* PlantProportion + MeatProportion = 1 */
constexpr double kRotFactor = 5.0;
>>>>>>> c4092f3 (Add optimization, fix shader, apply rot and grow)
constexpr double kGridCellSize = 50.0;
constexpr int kMinCreatureSize = 2;
constexpr double kReproductionThreshold = 0.80;
constexpr double kReproductionCooldown = 10;
constexpr int kInputNeurons = 12;
constexpr int kOutputNeurons = 6;
constexpr double kMaxNutritionalValue = 5;
constexpr double kDefaultLifespan = 30;
constexpr double kPhotosynthesisFactor = 0.1;
constexpr double kFrictionalCoefficient = 0.05;  // Needs to be less than 1
}  // namespace environment

namespace engine {
constexpr double kFixedUpdateInterval = 0.05;
constexpr double EPS = 1e-7;
constexpr size_t kMaxCellsToFindFood = 30;
// with the current function the max value is 1e-4 so we select a higher value
constexpr double kMaxFoodDensityColored = 5e-4;
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
constexpr double kDGestationRatioToIncubation = 10;
constexpr double kVisionRadius = 200;  // Default values or not used not sure
constexpr double kVisionAngle = M_PI / 3;  // Default values or not used not
                                           // sure
constexpr double kVisionARratio = 200 * M_PI / 3;
constexpr double kColorMutationFactor = 0.05;
constexpr double kDStomachCapacity = 2;
constexpr double kDDiet = 0.5;          // Default omnivores
constexpr double KDEatingCooldown = 1;  // Decrease for easier survival
constexpr double KDDigestionRate = 3;
constexpr double KDGeneticStrength = 0.6;
<<<<<<< HEAD
constexpr double KDAcidToEnergy = 5;  // Increase for easier survival

constexpr double kMaxRepdroducingAge = 700;
constexpr double kMatingDesireMaxProb = 0.05;
constexpr double kMatingDesireFactor = 0.05;
constexpr double kPregnancyEnergyFactor = 0.7;
constexpr double kPregnancyVelocityFactor = 0.5;
constexpr double kAfterBirthVelocityFactor = 1.5;
}  // namespace physical_constraints
=======
constexpr double KDAcidToEnergy = 5; // Increase for easier survival
constexpr double kBiteDamageRatio = 1;
constexpr double kBiteEnergyConsumptionRatio = 0.01;
} //namespace physical_constraints
>>>>>>> eac0cea (creation of the new bite function)

namespace ui {
constexpr double KDraggingSensitivity = 1.0f;
}  // namespace ui

}  // namespace settings

#endif  // CONFIG_H
