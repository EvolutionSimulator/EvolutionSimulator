#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    enum states {
        Dead = 0,
        Alive = 1
    };

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

    states GetState() const;
    void SetState (states state);
private:
    double x_coord_, y_coord_, orientation_, size_;
    states state_;
};

#endif // ENTITY_H
