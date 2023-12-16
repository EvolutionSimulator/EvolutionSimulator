#ifndef CONFIG_H
#define CONFIG_H

namespace settings {

    namespace neat {
        constexpr double kWeightMutationRate        = 0.2;
        constexpr double kStandardDeviationWeight   = 0.1;
        constexpr double kMaxWeight                 = 1.0;
        constexpr double kMinWeight                 = 0.0;
        constexpr double kAddNeuronMutationRate     = 0.05;
        constexpr double kAddLinkMutationRate       = 0.25;
        constexpr double kRemoveNeuronMutationRate  = 0.02;
        constexpr double kRemoveLinkMutationRate    = 0.15;
        constexpr double kChangeWeightMutationRate  = 0.6;
        constexpr double kBiasMutationRate          = 0.2;
        constexpr double kMaxBias                   = 0.1;
        constexpr double kMinBias                   = 0.0;
        constexpr double kChangeBiasMutationRate    = 0.6;
        constexpr double kAdjustmentProbability     = 0.8;
    }

    namespace environment {
        constexpr double kMapWidth                  = 1000.0;
        constexpr double kMapHeight                 = 300.0;
        constexpr double kCreatureDensity           = 0.0005;
        constexpr int    kMaxFoodSize               = 15;
        constexpr int    kMaxCreatureSize           = 15;
        constexpr double kTolerance                 = 1e-3;
        constexpr double kDefaultFoodDensity        = 0.005;
        constexpr double kEnergyToHealth            = 70.0;
        constexpr double kHealthToEnergy            = 10.0;
        constexpr double kDefaultCreatureDensity    = 0.0001;
        constexpr double kPlantNutritionalValue     = 2.0;
        constexpr double kMeatNutritionalValue      = 3.0;
        constexpr double kRotFactor                 = 1.0;
        constexpr double kGridCellSize              = 50.0;
        constexpr int    kMinCreatureSize           = 2;
        constexpr double kReproductionThreshold     = 0.90;
        constexpr double kReproductionCooldown      = 10;
        constexpr int    kInputNeurons              = 6;
        constexpr int    kOutputNeurons             = 2;
        constexpr double kMaxNutritionalValue       = 5;
        constexpr double kDefaultLifespan           = 30;
        constexpr double kPhotosynthesisFactor      = 0.1;

    }

    namespace engine {
        constexpr double kFixedUpdateInterval       = 0.05;
    }
}

#endif // CONFIG_H
