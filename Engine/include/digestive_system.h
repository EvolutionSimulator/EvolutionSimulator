#ifndef DIGESTIVE_SYSTEM_H
#define DIGESTIVE_SYSTEM_H

#include "alive_entity.h"
#include "food.h"

class DigestiveSystem : virtual public AliveEntity {
public:
  DigestiveSystem(neat::Genome genome, Mutable mutables);

  double GetStomachCapacity() const;
  double GetStomachFullness() const;
  double GetEmptinessPercent() const;
  double GetAcid() const;
  double GetEnergyInStomach() const;

  void SetStomachFullness(double value);
  void SetAcid(double value);

  void Digest(double deltaTime);
  void Bite(Food *food);
  void AddAcid(double quantity);
  void Eats(double nutritional_value);


protected:
  double stomach_capacity_; /*!< The stomach capacity (area) - mutable proportional to size squared */
  double stomach_fullness_; /*!< How much the stomach is filled (area) */
  double potential_energy_in_stomach_; /*! Energy to be gained in the stomach */
  double bite_strength_; /*! How much radius of things it can bite - - mutable proportional to size */
  double eating_cooldown_; /*! How much time it has to wait to bite - mutable*/
  double stomach_acid_; /*! Added when digestion occurs */
  bool biting_; /*! Indicates whether creature is biting or not*/
};



#endif // DIGESTIVE_SYSTEM_H
