#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    Entity();
    virtual ~Entity();

    double GetSize() const;
    void SetSize(double size);

    std::pair<double, double> GetCoordinates() const;
    void SetCoordinates(const double x_coord, const double y_coord);

private:
    double x_coord_, y_coord_, size_;
};

#endif // ENTITY_H
