#ifndef REPRODUCTIVE_SYSTEM_H
#define REPRODUCTIVE_SYSTEM_H

#include "alive_entity.h"

class ReproductiveSystem : virtual public AliveEntity {
public:
  ReproductiveSystem(neat::Genome genome, Mutable mutables);
  virtual ~ReproductiveSystem() override {}

  bool Fit();
  void Reproduced();


protected:
  bool fit_; /*!< Indicates whether the creature is fit in the evolutionary
                sense. */
  double reproduction_cooldown_; /*!< Cooldown period before the creature can
                                    reproduce again. */

};

#endif // REPRODUCTIVE_SYSTEM_H
