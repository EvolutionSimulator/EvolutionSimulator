#include "creature.h"

Creature::Creature(neat::Genome genome)
    : MovableEntity(), energy_(0.0), brain_(neat::NeuralNetwork(genome)), genome_(genome) {

}

double Creature::GetEnergy() const {
    return energy_;
}

void Creature::SetEnergy(double energy) {
    energy_ = energy;
}

void Creature::UpdateEnergy(){
    SetEnergy( GetEnergy() - (GetVelocityForward() + GetRotationalVelocity()) * GetSize());

    if (GetEnergy() <= 0){
        Dies();
    }
}

void Creature::Dies(){
    SetState(Dead);
}

void Creature::Eats(double nutritional_value){
    SetEnergy(GetEnergy() + nutritional_value);
}

bool Creature::Fit(){
    if (energy_ > cfg::reproduction_threshold){
        return true;
    }
    return false;
}

void Creature::Update(double deltaTime, double const kMapWidth, double const kMapHeight){
    this->Move(deltaTime, kMapWidth,  kMapHeight);
    this->Rotate(deltaTime);
}

neat::Genome Creature::GetGenome(){
    return genome_;
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
