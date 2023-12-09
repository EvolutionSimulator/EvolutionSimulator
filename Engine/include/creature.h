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
    void UpdateEnergy(const double energyToHealth, const double healthToEnergy);
    double GetEnergy() const;
    void SetEnergy(double energy);

    double GetHealth() const;
    void SetHealth(double health);

    void HealthToEnergy();
    void EnergyToHealth();

    virtual void OnCollision(Food& food);
    virtual void OnCollision(Creature& creature);
    bool Fit();

protected:
    double energy_;
    double health_;
    neat::NeuralNetwork brain_;
    neat::Genome genome_;
};

#endif // CREATURE_HPP
