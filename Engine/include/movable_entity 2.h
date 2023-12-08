#ifndef MOVABLE_ENTITY_H
#define MOVABLE_ENTITY_H

#include "entity.h"

class MovableEntity : public Entity {
public:
    MovableEntity();

    double GetVelocityForward() const;
    double GetRotationalVelocity() const;

    void SetVelocityForward(double velocity);
    void SetRotationalVelocity(double rotational_velocity);

    void Move(double deltaTime);
    void Rotate(double deltaTime);

private:
    double velocity_forward_;
    double rotational_velocity_;
};

#endif // CREATURE_HPP
