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

// called when the widget is first rendered
void SimulationCanvas::OnInit() {
    clear(sf::Color(0, 255, 0));
}

// called on each repaint
void SimulationCanvas::OnUpdate() {
    simulation_->ProcessData(render_lambda_);
}

// use this to process the simulation data and render it on the screen
void SimulationCanvas::RenderSimulation(SimulationData* data)
{
    clear(sf::Color(0, 255, 0));

    // Iterate through entities and create a circle shape for each
    for (const auto& entity : data->entities_) {
        sf::CircleShape shape(20);

        std::pair<double, double> coordinates = entity.GetCoordinates();
        shape.setPosition(coordinates.first, coordinates.second);

        draw(shape);
    }
}
