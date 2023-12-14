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
        sf::VertexArray foodShape = createGradientCircle(food.GetSize(), sf::Color(50, 100, 49), sf::Color(150, 250, 148));

        std::pair<double, double> foodCoordinates = food.GetCoordinates();
        sf::Transform foodTransform;
        foodTransform.translate(foodCoordinates.first, foodCoordinates.second);
        draw(foodShape, foodTransform);
    }

    // Iterate through creatures and create a gradient circle shape for each
    for (const auto& creature : data->creatures_) {
        sf::VertexArray creatureShape = createGradientCircle(creature.GetSize(), sf::Color(79, 77, 64), sf::Color(41, 40, 38));

        std::pair<double, double> creatureCoordinates = creature.GetCoordinates();
        sf::Transform creatureTransform;
        creatureTransform.translate(creatureCoordinates.first, creatureCoordinates.second);
        draw(creatureShape, creatureTransform);
    }
}

void SimulationCanvas::DrawGraph(sf::RenderWindow& window, double (*graphFunction)(double), double xMin, double xMax, double yMin, double yMax)
{
    // Set the coordinate system to match the window dimensions
    window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));

    // Render the graph within the box
    sf::VertexArray line(sf::LinesStrip);
    double scaleFactor = 0.6; // Adjust as needed

    // Calculate the starting x-coordinate to center the graph
    double startX = (window.getSize().x - scaleFactor * window.getSize().x) / 2;

    // Calculate the starting y-coordinate to center the graph vertically within the box
    double startY = (window.getSize().y - scaleFactor * window.getSize().y) / 2;

    // Calculate the x-increment based on the range and resolution
    double xIncrement = (xMax - xMin) / (scaleFactor * window.getSize().x);

    for (double x = xMin; x <= xMax; x += xIncrement) {
        double y = graphFunction(x);

        // Map graph coordinates to window coordinates
        double mappedX = startX + (x - xMin) / (xMax - xMin) * scaleFactor * window.getSize().x;
        double mappedY = startY + (y - yMin) / (yMax - yMin) * scaleFactor * window.getSize().y;

        // Draw points
        line.append(sf::Vertex(sf::Vector2f(mappedX, mappedY), sf::Color::Blue));
    }

    // Draw a square black border around the graph area
    sf::RectangleShape borderRect;
    borderRect.setSize(sf::Vector2f(scaleFactor * window.getSize().x, scaleFactor * window.getSize().y));
    borderRect.setOutlineColor(sf::Color::Black);
    borderRect.setOutlineThickness(2.0);
    borderRect.setPosition(sf::Vector2f(startX, startY)); // Adjust as needed
    window.draw(borderRect);

    // Draw the graph
    window.draw(line);
}

void SimulationCanvas::DrawCreatureCountOverTime(sf::RenderWindow& window, const std::vector<Creature>& creatures)
{
    // Set the coordinate system to match the window dimensions
    window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));

    // Render the creature count over time graph within the box
    sf::VertexArray line(sf::LinesStrip);
    double scaleFactor = 0.6; // Adjust as needed

    // Calculate the starting x-coordinate to center the graph
    double startX = (window.getSize().x - scaleFactor * window.getSize().x) / 2;

    // Calculate the starting y-coordinate to center the graph vertically within the box
    double startY = (window.getSize().y - scaleFactor * window.getSize().y) / 2;

    // Declaration for left ticks
    sf::VertexArray leftTicks(sf::Lines);

    // Declaration for bottom ticks
    sf::VertexArray bottomTicks(sf::Lines);

    // Calculate the x-increment based on the range and resolution
    double xIncrement = 1.0; // Assuming a time step of 1, adjust as needed

    for (std::size_t i = 0; i < creatures.size(); ++i) {
        // Use creatures.size() directly to get the count
        double creatureCount = creatures.size();

        // Map graph coordinates to window coordinates
        double mappedX = startX + i * xIncrement / creatures.size() * scaleFactor * window.getSize().x;
        double mappedY = startY + (creatureCount - 0.0) / (creatures.size() - 0.0) * scaleFactor * window.getSize().y;

        // Draw points
        line.append(sf::Vertex(sf::Vector2f(mappedX, mappedY), sf::Color::Blue));
    }

    // Draw a square black border around the graph area
    sf::RectangleShape borderRect;
    borderRect.setSize(sf::Vector2f(scaleFactor * window.getSize().x, scaleFactor * window.getSize().y));
    borderRect.setOutlineColor(sf::Color::Black);
    borderRect.setOutlineThickness(2.0);
    borderRect.setPosition(sf::Vector2f(startX, startY)); // Adjust as needed
    window.draw(borderRect);

    int numTicks = 5; // Adjust as needed
    double tickLength = 5.0; // Adjust as needed

    // Draw ticks on the left and add labels
    for (double y = startY; y <= startY + scaleFactor * window.getSize().y; y += scaleFactor * window.getSize().y / numTicks) {
        leftTicks.append(sf::Vertex(sf::Vector2f(startX - tickLength, y), sf::Color::Black));
        leftTicks.append(sf::Vertex(sf::Vector2f(startX, y), sf::Color::Black));

        // Add labels
        sf::Text label;
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::Black);
        label.setString(std::to_string(static_cast<int>((y - startY) / (scaleFactor * window.getSize().y) * creatures.size())));
        label.setPosition(startX - tickLength - 30, y - 6); // Adjust as needed
        window.draw(label);
    }

    // Draw ticks on the bottom and add labels
    for (double x = startX; x <= startX + scaleFactor * window.getSize().x; x += scaleFactor * window.getSize().x / numTicks) {
        bottomTicks.append(sf::Vertex(sf::Vector2f(x, startY + scaleFactor * window.getSize().y), sf::Color::Black));
        bottomTicks.append(sf::Vertex(sf::Vector2f(x, startY + scaleFactor * window.getSize().y + tickLength), sf::Color::Black));

        // Add labels
        sf::Text label;
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::Black);
        label.setString(std::to_string(static_cast<int>((x - startX) / (scaleFactor * window.getSize().x) * creatures.size())));
        label.setPosition(x - 15, startY + scaleFactor * window.getSize().y + tickLength);
        window.draw(label);
    }

    // Draw the ticks
    window.draw(leftTicks);
    window.draw(bottomTicks);

    // Draw the graph
    window.draw(line);
}
