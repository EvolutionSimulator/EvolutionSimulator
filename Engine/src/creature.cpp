#include "creature.h"
#include "config.h"

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

Creature::states Creature::GetState() const{
    return state_;
}

void Creature::SetState(states state){
    state_ = state;
}

void Creature::Update(double deltaTime){
    this->Rotate(deltaTime);
    this->Move(deltaTime);
}

bool Creature::Fit(){
    if (energy_ > cfg::reproduction_threshold){
        return true;
    }
    return false;
}

neat::Genome Creature::GetGenome() const{
    return genome_;
}
