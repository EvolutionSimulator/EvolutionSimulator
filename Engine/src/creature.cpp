#include "creature.h"

Creature::Creature()
    : MovableEntity(), energy_(0.0) {

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
