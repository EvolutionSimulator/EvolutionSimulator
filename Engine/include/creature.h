#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"

class Creature : public MovableEntity {
public:
    enum states {
        Dead = 0,
        Alive = 1
    };
    Creature();

    void Dies();
    void Eats(double nutritional_value);
    void UpdateEnergy();
    double GetEnergy() const;
    void SetEnergy(double energy);
    void Update(double deltaTime);

    states GetState() const;
    void SetState (states state);

private:
    double energy_;
    states state_;
};

#endif // CREATURE_HPP
