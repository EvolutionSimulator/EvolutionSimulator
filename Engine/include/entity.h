#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    Entity();
    Entity(const double x_coord, const double y_coord, const double size);
    virtual ~Entity();

    double GetSize() const;
    void SetSize(double size);

    std::pair<double, double> GetCoordinates() const;
    void SetCoordinates(const double x_coord, const double y_coord);

    void RandomInitialization(const double world_size);

private:
    double x_coord_, y_coord_, size_;
};

#endif // ENTITY_H
