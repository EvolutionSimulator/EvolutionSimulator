#include "neat/neat-genome.h"
#include "reproductive_system.h"
#include "settings.h"

ReproductiveSystem::ReproductiveSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables),
      reproduction_cooldown_ (mutables.GetMaturityAge())
{

}


/*!
 * @brief Checks if the creature is fit for reproduction.
 *
 * @details This method determines if the creature meets the energy requirements
 * and cooldown period for reproduction.
 *
 * @return true if the creature is fit for reproduction, false otherwise.
 */
bool ReproductiveSystem::Fit() {
  if (energy_ > SETTINGS.environment.reproduction_threshold * max_energy_ &&
      reproduction_cooldown_ == 0.0 && age_ < 700) {
    return true;
  }
  return false;
}

/*!
 * @brief Handles the creature's reproduction process.
 *
 * @details This method deducts the energy cost of reproduction and initiates
 * the cooldown period.
 */
void  ReproductiveSystem::Reproduced() {
  SetEnergy(GetEnergy() - 0.7 * max_energy_);
  reproduction_cooldown_ = mutable_.GetReproductionCooldown();
}
