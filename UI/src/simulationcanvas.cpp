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
    clear(sf::Color(9, 109, 6));

    // Iterate through food and create a circle shape for each
    for (const auto& food : data->food_entities_) {
        sf::CircleShape foodShape(5);
        sf::Color myColor(100, 200, 98);
        foodShape.setFillColor(myColor);

        std::pair<double, double> foodCoordinates = food.GetCoordinates();
        foodShape.setPosition(foodCoordinates.first, foodCoordinates.second);
        draw(foodShape);
    }

    // Iterate through creatures and create a circle shape for each
    for (const auto& creature : data->creatures_) {
        sf::CircleShape shape(10);
        sf::Color myColor(89, 109, 30);
        shape.setFillColor(myColor);
        std::pair<double, double> coordinates = creature.GetCoordinates();
        shape.setPosition(coordinates.first, coordinates.second);

        draw(shape);
    }
}
