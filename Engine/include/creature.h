#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"

class Creature : public MovableEntity {
public:

    Creature();

    void Dies();
    void Eats(double nutritional_value);
    void UpdateEnergy();
    double GetEnergy() const;
    void SetEnergy(double energy);

    virtual void OnCollision(Food& food);
    virtual void OnCollision(Creature& creature);

private:
    double energy_;
};

#endif // CREATURE_HPP
