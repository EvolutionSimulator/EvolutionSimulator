#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : public Food, public Creature {
public:
    Egg(neat::Genome genome, Mutable mutable_, const double x_coord, const double y_coord);
    void Break();
    Creature Hatch();
protected:
    int incubation_time_;
    double age_;
    double size_;
    bool fertilized_;
    std::pair<double,double> coordinates_;
};

#endif // EGG_H
