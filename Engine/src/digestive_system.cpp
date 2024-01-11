#include "digestive_system.h"
#include "settings.h"

DigestiveSystem::DigestiveSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables),
      eating_cooldown_ (mutables.GetEatingSpeed()),
      stomach_acid_ (0.0),
      potential_energy_in_stomach_(0.0)
{
  stomach_capacity_ = mutables.GetStomachCapacityFactor() * pow(size_, 2);
  bite_strength_ = mutables.GetGeneticStrength() * size_;
}

/*!
 * @brief Handles the creature's consumption of food.
 *
 * @details Increases the creature's energy based on the nutritional value of
 * the food consumed. If the resulting energy exceeds the maximum energy limit,
 * it triggers a health balance routine.
 *
 * @param nutritional_value The nutritional value of the consumed food.
 */
void DigestiveSystem::Eats(double nutritional_value) {
  //velocity_ = 0;
  SetEnergy(GetEnergy() + nutritional_value);
  if (GetEnergy() > max_energy_) {
    BalanceHealthEnergy();
  }
}

double DigestiveSystem::GetStomachCapacity() const {return stomach_capacity_;};
double DigestiveSystem::GetStomachFullness() const {return stomach_fullness_;};
double DigestiveSystem::GetEmptinessPercent() const {return 100 * (1 - stomach_fullness_/stomach_capacity_);};
double DigestiveSystem::GetEnergyInStomach() const {return potential_energy_in_stomach_;};
double DigestiveSystem::GetAcid() const {return stomach_acid_; };


/*!
 * @brief Handles the creature's digestion of food.
 *
 * @details Increases the creature's energy based on the nutritional value of
 * the food digested. If the resulting energy exceeds the maximum energy limit,
 * it triggers a health balance routine. Then empties out stomach.
 *
 * @param nutritional_value The nutritional value of the food to be digested.
 */
void DigestiveSystem::Digest(double deltaTime)
{

  double quantity = std::min(deltaTime * SETTINGS.physical_constraints.d_digestion_rate, stomach_acid_);
  quantity = std::min(quantity,  stomach_fullness_);

  if (quantity < SETTINGS.engine.eps || stomach_fullness_ < SETTINGS.engine.eps) { return; };
  double avg_nutritional_value = potential_energy_in_stomach_ / stomach_fullness_;

         // Digests the food, increasing energy
  SetEnergy(GetEnergy() + quantity * avg_nutritional_value);
  if (GetEnergy() > max_energy_) {
    BalanceHealthEnergy();
  }

         // Empties out the stomach space
  SetAcid(GetAcid() - quantity);
  potential_energy_in_stomach_ -= quantity * avg_nutritional_value;
  SetStomachFullness(GetStomachFullness() - quantity);
}

/*!
 * @brief Handles the biting of the food.
 *
 * @details Adds the food it bites to the stomach (increasing fulness and potential
 * energy). Decreases food size/deletes food that gets bitten.
 *
 * @param food The food the creature bites into.
 */
void DigestiveSystem::Bite(Food* food)
{
  //Reset eating cooldown, makes creature stop to bite
  eating_cooldown_ = mutable_.GetEatingSpeed();

         //Bite logic
  double max_nutrition = 0;

         //Check how much food the creature can eat, depending on bite strength and fullness of stomach
  double available_space = std::max(stomach_capacity_ - stomach_fullness_, 0.0);

  double area_to_eat = std::min(M_PI*pow(bite_strength_,2), available_space);
  area_to_eat = std::max(area_to_eat,0.0);
  double food_to_eat = std::sqrt(area_to_eat);

         // Check if creature eats the whole food or a part of it
  if (food_to_eat >= food->GetSize())
  {
    max_nutrition = food->GetNutritionalValue() * food->GetSize();
    SetStomachFullness(GetStomachFullness()+  M_PI*pow(food->GetSize(), 2));
    food->Eat();
  }
  else
  {
    double initial_food_size = food->GetSize();
    double new_radius = std::sqrt(std::abs(pow(initial_food_size,2) - pow(food_to_eat,2)));
    food->SetSize(new_radius);
    SetStomachFullness(GetStomachFullness()+ M_PI*pow(food_to_eat, 2));
    max_nutrition =  food->GetNutritionalValue() * food_to_eat;
  }

         // Herbivore/carnivore multiplier
  if (Plant *plant = dynamic_cast<Plant *>(food)) {
    max_nutrition = max_nutrition * 2 * (1 - mutable_.GetDiet());
  }
  else if (Meat *meat = dynamic_cast<Meat *>(food)) {
    max_nutrition = max_nutrition * 2 * mutable_.GetDiet();
  }

         //Add nutrition to stomach, make sure capacity is not surpassed
  potential_energy_in_stomach_ += max_nutrition;
}

void DigestiveSystem::AddAcid(double quantity)
{
  double initial_acid = stomach_acid_;
  stomach_acid_ = std::min(stomach_capacity_, stomach_acid_ + quantity);
  SetEnergy(GetEnergy() - (stomach_acid_ - initial_acid)/SETTINGS.physical_constraints.d_acid_to_energy);
}

void DigestiveSystem::SetStomachFullness(double value)
{
  if (value > stomach_capacity_)
  {
    stomach_fullness_ = stomach_capacity_;
  }
  else{
    if (value < 0) { stomach_fullness_ = 0;}
    else { stomach_fullness_ = value; }
  }
}

void DigestiveSystem::SetAcid(double value)
{
  if (value > stomach_capacity_)
  {
    stomach_acid_ = stomach_capacity_;
  }
  else{
    if (value < 0) { stomach_acid_ = 0;}
    else { stomach_acid_ = value; }
  }
}
