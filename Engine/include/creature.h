#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"
#include "neat/neat-neural-network.h"

class Creature : public MovableEntity {
public:

    Creature(neat::Genome genome);

    void Dies();
    void Eats(double nutritional_value);
    void UpdateEnergy();
    double GetEnergy() const;
    void SetEnergy(double energy);

    virtual void OnCollision(Food& food);
    virtual void OnCollision(Creature& creature);

    bool Fit();

private:
    double energy_;
    neat::NeuralNetwork brain_;
    neat::Genome genome_;
};

#endif // CREATURE_HPP
