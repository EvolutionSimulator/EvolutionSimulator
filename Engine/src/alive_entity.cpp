#include "alive_entity.h"

#include "settings.h"

AliveEntity::AliveEntity(neat::Genome genome, Mutable mutables)
    : Entity(),
      mutable_(mutables),
      brain_(neat::NeuralNetwork(genome)),
      genome_(genome),
      age_(0) {
    size_ = mutables.GetBabySize();
    health_ = mutables.GetIntegrity() * pow(size_, 2)/2;
    energy_ = mutables.GetEnergyDensity() * pow(size_, 2)/2;
    int neural_inputs = SETTINGS.environment.input_neurons;
    for (BrainModule module : genome.GetModules()){
        neural_inputs += module.GetInputNeuronIds().size();
    }
    neural_inputs_ = neural_inputs;
}

/*!
 * @brief Retrieves the generation of the AliveEntity.
 *
 * @details This function returns the generation number of the AliveEntity
 * instance. The generation is stored as a private member variable `generation_`
 * within the AliveEntity class. This function is a constant member function,
 * implying it does not modify any member variables.
 *
 * @return int The generation number of the AliveEntity.
 */
int AliveEntity::GetGeneration() const { return generation_; }

/*!
 * @brief Sets the generation number for the AliveEntity.
 *
 * @details This method assigns a new generation number to the AliveEntity.
 * The generation number typically represents the AliveEntity's place in a
 * generational sequence.
 *
 * @param generation The generation number to be set for the AliveEntity.
 */
void AliveEntity::SetGeneration(int generation) { generation_ = generation; }

/*!
 * @brief Retrieves the current energy level of the AliveEntity.
 *
 * @details This method returns the AliveEntity's current energy, which is a key
 * factor in its survival and activities.
 *
 * @return double The current energy level of the AliveEntity.
 */
double AliveEntity::GetEnergy() const { return energy_; }

/*!
 * @brief Balances the AliveEntity's health and energy levels based on various
 * conditions.
 *
 * @details This method adjusts the AliveEntity's health and energy depending on
 * their current values and predefined thresholds.
 */
void AliveEntity::BalanceHealthEnergy() {
  if (GetEnergy() < 0) {
    SetHealth(GetHealth() + GetEnergy() - 0.1);
    SetEnergy(0.1);
  } else if (GetHealth() < GetEnergy() && GetEnergy() >= 0.1 * max_energy_) {
    SetEnergy(GetEnergy() - 0.1);
    SetHealth(GetHealth() + 0.1);
  }
}

/*!
 * @brief Retrieves the current health level of the AliveEntity.
 *
 * @details This method returns the AliveEntity's current health, reflecting its
 * overall well-being.
 *
 * @return double The current health level of the AliveEntity.
 */
double AliveEntity::GetHealth() const { return health_; }

/*!
 * @brief Sets the health level of the AliveEntity.
 *
 * @param health The new health level to be set, capped at 100.
 */
void AliveEntity::SetHealth(double health) {
  if (health > mutable_.GetIntegrity() * pow(size_, 2)) {
    health_ = mutable_.GetIntegrity() * pow(size_, 2);
  } else {
    health_ = health;
  }
}

/*!
 * @brief Triggers the death of the AliveEntity.
 *
 * @details This method changes the state of the AliveEntity to 'Dead',
 * indicating its demise.
 */
void AliveEntity::Dies() { SetState(Dead); }

/*!
 * @brief Sets the energy level of the AliveEntity.
 *
 * @param energy The new energy level to be set, limited by the AliveEntity's
 * maximum energy.
 */
void AliveEntity::SetEnergy(double energy) {
  if (energy > max_energy_) {
    energy_ = max_energy_;
  } else {
    energy_ = energy;
  }
}

/*!
 * @brief Retrieves the maximum energy level of the AliveEntity.
 *
 * @details This method returns the upper limit of the AliveEntity's energy
 * capacity.
 *
 * @return double The maximum energy level of the AliveEntity.
 */
double AliveEntity::GetMaxEnergy() const { return max_energy_; }

/*!
 * @brief Sets  the maximum energy level of the AliveEntity.
 *
 * @details This method sets the upper limit of the AliveEntity's energy
 * capacity.
 */
void AliveEntity::SetMaxEnergy(double max_energy) { max_energy_ = max_energy; };

/*!
 * @brief Updates the maximum energy level of the AliveEntity.
 *
 * @details This method takes into account the changes in the energy
 *          level due to both the increased size and the age.
 *
 * @param max_energy The desired maximum energy level.
 */
void AliveEntity::UpdateMaxEnergy() {
  max_energy_ = mutable_.GetEnergyDensity() * pow(size_, 2) * std::exp(-age_/50);
}

/*!
 * @brief Retrieves the age of the AliveEntity.
 *
 * @details This method returns the current age of the AliveEntity, which is
 * stored in the age_ member variable.
 *
 * @return The current age of the AliveEntity.
 */
double AliveEntity::GetAge() const { return age_; }

/*!
 * @brief Sets the age of the AliveEntity.
 *
 * @param age The new age to be set.
 */
void AliveEntity::SetAge(double age) { age_ = age; }

/*!
 * @brief Updates the age of the AliveEntity and its health.
 */

void AliveEntity::UpdateAge(double delta_time) {
    age_ += delta_time;
    health_ -= delta_time;
}

/*!
 * @brief Retrieves the AliveEntity's genetic genome.
 *
 * @details Returns the genome of the AliveEntity, which is a representation
 * of its genetic makeup.
 *
 * @return The genome of the AliveEntity.
 */
neat::Genome AliveEntity::GetGenome() const { return genome_; }

/*!
 * @brief Retrieves the AliveEntity's mutables
 *
 * @details Return the mutables of the AliveEntity which is a representation
 * of its characteristics
 *
 * @return The mutables of the AliveEntity.
 */
Mutable AliveEntity::GetMutable() const { return mutable_; }
