#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"

class Creature : public MovableEntity {
public:
    Creature();

    double GetEnergy() const;
    void SetEnergy(double energy);

private:
    double energy_;
};

#endif // CREATURE_HPP
