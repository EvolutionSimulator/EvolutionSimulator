#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"

class Creature : public MovableEntity {
public:

    Creature();

    void Dies();
    void Eats(double nutritional_value);
    void UpdateEnergy(const double energyToHealth, const double healthToEnergy);
    double GetEnergy() const;
    void SetEnergy(double energy);

    double GetHealth() const;
    void SetHealth(double health);

    void HealthToEnergy();
    void EnergyToHealth();

    virtual void OnCollision(Food& food);
    virtual void OnCollision(Creature& creature);

protected:
    double energy_;
    double health_;
};

#endif // CREATURE_HPP
