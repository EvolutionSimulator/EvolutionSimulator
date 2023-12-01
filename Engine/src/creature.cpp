#include "creature.h"

Creature::Creature()
    : MovableEntity()
{

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
