#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "movable_entity.h"
#include "food.h"
#include "neat/neat-neural-network.h"

class Creature : public MovableEntity {
public:
    enum states {
        Dead = 0,
        Alive = 1,
    };
    Creature();
    Creature(neat::Genome genome);
    void Dies();
    void Eats(double nutritional_value);
    void UpdateEnergy();
    double GetEnergy() const;
    void SetEnergy(double energy);
    void Update(double deltaTime);
    bool Fit();

    states GetState() const;
    void SetState (states state);

    neat::Genome GetGenome() const;

protected:
    double energy_;
    states state_;
    neat::NeuralNetwork brain_;
    neat::Genome genome_;
};

#endif // CREATURE_HPP
