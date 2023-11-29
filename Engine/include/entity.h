#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    Entity();
    Entity(const double x_coord, const double y_coord, const double size);
    virtual ~Entity();

    void RandomInitialization(const double world_width, const double world_height);

    double GetSize() const;
    void SetSize(double size);

    std::pair<double, double> GetCoordinates() const;
    void SetCoordinates(const double x_coord, const double y_coord);

    double GetOrientation() const;
    void SetOrientation(double orientation);

    double GetDistance(const Entity& otherEntity) const;

private:
    double x_coord_, y_coord_, orientation_, size_;
};

#endif // ENTITY_H
