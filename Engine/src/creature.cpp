#include "creature.h"

Creature::Creature(neat::Genome genome)
    : MovableEntity(), health_(100), energy_(100), brain_(neat::NeuralNetwork(genome)), genome_(genome) {
}

double Creature::GetEnergy() const {
    return energy_;
}

void Creature::SetEnergy(double energy) {
    if (energy > 100) {
        energy_ = 100;
    } else {
        energy_ = energy;
    }
}

void Creature::UpdateEnergy(const double energyToHealth, const double healthToEnergy){
    SetEnergy( GetEnergy() - (GetVelocityForward() + GetRotationalVelocity()) * GetSize());

    if (GetEnergy() <= healthToEnergy){
        HealthToEnergy();
    } else if (GetEnergy() >= energyToHealth){
        EnergyToHealth();
    }

    if (GetHealth() <= 0){
        Dies();
    }
}

void Creature::HealthToEnergy() {
    if (GetEnergy() < 0) {
        SetHealth(GetHealth()- GetEnergy()-5);
        SetEnergy(5);
    }
    SetEnergy(GetEnergy() + 5);
    SetHealth(GetHealth() - 5);
}

void Creature::EnergyToHealth() {
    SetEnergy(GetEnergy() - 5);
    SetHealth(GetHealth() + 5);
}

double Creature::GetHealth() const {
    return health_;
}

void Creature::SetHealth(double health) {
    if (health > 100) {
        health_ = 100;
    } else {
        health_ = health;
    }
}

void Creature::Dies(){
    SetState(Dead);
}

void Creature::Eats(double nutritional_value){
    SetEnergy(GetEnergy() + nutritional_value);
    if (GetEnergy() > 100) {
        EnergyToHealth();
    }}

bool Creature::Fit(){
    if (energy_ > cfg::reproduction_threshold){
        return true;
    }
    return false;
}

void Creature::OnCollision(Food& food)
{
    if (food.GetState() == Entity::Alive)
    {
        Eats(food.GetNutritionalValue());
        food.Eat();
    }
}


void Creature::OnCollision(Creature& creature)
{

}
