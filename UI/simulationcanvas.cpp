#include "simulationcanvas.h"
#include <iostream>

SimulationCanvas::SimulationCanvas(QWidget* Parent) :
    QSFMLCanvas(Parent)
{ }

void SimulationCanvas::SetSimulation(Simulation* simulation)
{
    simulation_ = simulation;
}

Simulation *SimulationCanvas::GetSimulation()
{
    return simulation_;
}

void SimulationCanvas::OnInit() {
    clear(sf::Color(0, 255, 0));
}

void SimulationCanvas::OnUpdate() {
    clear(sf::Color(0, 255, 0));
    auto shape1 = sf::CircleShape(20, 100);
    shape1.setPosition(simulation_->x, 0);
    auto shape2 = sf::CircleShape(20, 100);
    shape2.setPosition(simulation_->y, 45);
    draw(shape1);
    draw(shape2);
}
