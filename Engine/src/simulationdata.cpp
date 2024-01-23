#include "simulationdata.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "collision_functions.h"
#include "food.h"
#include "mathlib.h"
#include "settings.h"

/*!
 * @brief Retrieves the current environment of the simulation.
 *
 * @return Returns the simulation's current environment.
 */

Environment SimulationData::GetEnvironment() { return environment_; }

/*!
 * @brief Sets the environment for the simulation.
 *
 * @param environment The new environment to be set for the simulation.
 */

void SimulationData::SetEnvironment(Environment& environment) {
  environment_ = environment;
}

void SimulationData::UpdateStatistics() {
  double current_time = world_time_;
  if (current_time - lastRecordedTime_ >= 1.0) {
    lastRecordedTime_ = current_time;
    creatureCountOverTime_.push_back(creatures_.size());
    double average_size = 0.0;
    double average_energy = 0.0;
    double average_velocity = 0.0;
    for (const auto creature : creatures_) {
      average_size += creature->GetSize();
      average_energy += creature->GetEnergy();
      average_velocity += creature->GetVelocity();
    }
    average_size /= creatures_.size();
    average_energy /= creatures_.size();
    average_velocity /= creatures_.size();
    creatureSizeOverTime_.push_back(average_size);
    creatureEnergyOverTime_.push_back(average_energy);
    creatureVelocityOverTime_.push_back(average_velocity);
  }
}

std::vector<int> SimulationData::GetCreatureCountOverTime() const {
  return creatureCountOverTime_;
}

std::vector<double> SimulationData::GetCreatureSizeOverTime() const {
  return creatureSizeOverTime_;
}

std::vector<double> SimulationData::GetCreatureEnergyOverTime() const {
  return creatureEnergyOverTime_;
}

std::vector<double> SimulationData::GetCreatureVelocityOverTime() const {
  return creatureVelocityOverTime_;
}
