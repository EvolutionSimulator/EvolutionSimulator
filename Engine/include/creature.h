#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "entity.h"

class Creature : public Entity {
public:
    Creature();

    double GetEnergy() const;
    double GetVelocityForward() const;
    double GetRotationalVelocity() const;

    void SetEnergy(double energy);
    void SetVelocityForward(double velocity);
    void SetRotationalVelocity(double rotational_velocity);

private:
    double energy_;
    double velocity_forward_;
    double rotational_velocity_;
};

#endif // CREATURE_HPP