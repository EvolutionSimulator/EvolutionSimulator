#include "simulationcanvas.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

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

sf::VertexArray createGradientCircle(float radius, const sf::Color& centerColor, const sf::Color& edgeColor) {
    const int points = 100;
    sf::VertexArray circle(sf::TriangleFan, points + 2);

    circle[0].position = sf::Vector2f(radius, radius);
    circle[0].color = centerColor;

    for (int i = 1; i <= points + 1; ++i) {
        float angle = (i - 1) * (2 * M_PI / points);
        sf::Vector2f point(radius * cos(angle) + radius, radius * sin(angle) + radius);

        // Distance from center
        float dist = std::sqrt(std::pow(point.x - radius, 2) + std::pow(point.y - radius, 2)) / radius;
        sf::Color color(
            static_cast<sf::Uint8>(centerColor.r * (1 - dist) + edgeColor.r * dist),
            static_cast<sf::Uint8>(centerColor.g * (1 - dist) + edgeColor.g * dist),
            static_cast<sf::Uint8>(centerColor.b * (1 - dist) + edgeColor.b * dist)
            );

        circle[i].position = point;
        circle[i].color = color;
    }

    return circle;
}

// use this to process the simulation data and render it on the screen
void SimulationCanvas::RenderSimulation(SimulationData* data)
{
    clear(sf::Color(9, 109, 6));

    // Iterate through food and create a gradient circle shape for each
    for (const auto& food : data->food_entities_) {
        sf::VertexArray foodShape = createGradientCircle(5, sf::Color(50, 100, 49), sf::Color(150, 250, 148));

        std::pair<double, double> foodCoordinates = food.GetCoordinates();
        sf::Transform foodTransform;
        foodTransform.translate(foodCoordinates.first, foodCoordinates.second);
        draw(foodShape, foodTransform);
    }

    // Iterate through creatures and create a gradient circle shape for each
    for (const auto& creature : data->creatures_) {
        sf::VertexArray creatureShape = createGradientCircle(10, sf::Color(79, 77, 64), sf::Color(41, 40, 38));

        std::pair<double, double> creatureCoordinates = creature.GetCoordinates();
        sf::Transform creatureTransform;
        creatureTransform.translate(creatureCoordinates.first, creatureCoordinates.second);
        draw(creatureShape, creatureTransform);
    }
}
