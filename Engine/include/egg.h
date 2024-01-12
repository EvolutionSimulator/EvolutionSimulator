#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "movable_entity.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : public MovableEntity, public Food {
public:
    Egg(neat::Genome genome, Mutable mutable_, std::pair<double,double> coords);
    void Break();
    Creature Hatch();
protected:
    int incubation_time_;
    double age_;
    double size_;
    bool fertilized_;
    neat::Genome genome_;
    Mutable mutables_;
    std::pair<double,double> coordinates_;
};

#endif // EGG_H
