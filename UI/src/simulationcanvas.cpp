#include "simulationcanvas.h"
#include <iostream>

SimulationCanvas::SimulationCanvas(QWidget* Parent) :
    QSFMLCanvas(Parent)
{
    render_lambda_ = [this](SimulationData* data) {
        this->RenderSimulation(data);
    };
}

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
    simulation_->ProcessData(render_lambda_);
}

void SimulationCanvas::RenderSimulation(SimulationData* data)
{
    clear(sf::Color(0, 255, 0));
    auto shape1 = sf::CircleShape(20, 100);
    shape1.setPosition(data->x, 0);
    auto shape2 = sf::CircleShape(20, 100);
    shape2.setPosition(data->y, 45);
    draw(shape1);
    draw(shape2);
}
