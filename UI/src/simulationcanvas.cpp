#include "simulationcanvas.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QUuid>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <QMouseEvent>
#include <QPainter>
#include <sstream>


SimulationCanvas::SimulationCanvas(QWidget* Parent) : QSFMLCanvas(Parent), showInfoPanel(false) {
    render_lambda_ = [this](SimulationData* data) {
        this->RenderSimulation(data);
    };

    QFile resourceFile(":/font.ttf");
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open font resource!";
        return;
    }

    // Generate a unique temporary filename
    QString tempFileName = "temp_font_" + QUuid::createUuid().toString(QUuid::WithoutBraces) + ".ttf";
    QString tempFilePath = QDir::temp().absoluteFilePath(tempFileName);
    qDebug() << "Temporary file path:" << tempFilePath;

    QFile tempFile(tempFilePath);
    if (tempFile.exists()) {
        qDebug() << "Temporary file already exists. Deleting...";
        if (!tempFile.remove()) {
            qDebug() << "Failed to remove existing temporary file.";
            return;
        }
    }

    if (!resourceFile.copy(tempFilePath)) {
        qDebug() << "Failed to copy font to temporary file!";
        qDebug() << "Error:" << resourceFile.errorString();
        return;
    }
    resourceFile.close();

    if (!font_.loadFromFile(tempFilePath.toStdString())) {
        qDebug() << "Failed to load font from file!";
        QFile::remove(tempFilePath);
        return;
    }

    // Clean up the temporary file after use
    QFile::remove(tempFilePath);
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

void SimulationCanvas::OnUpdate() {
    simulation_->ProcessData(render_lambda_);
    RenderSimulation(simulation_->GetSimulationData());

    if (showInfoPanel && clickedCreaturePos) {
        sf::Vector2f panelSize(200.f, 100.f);
        sf::Vector2f panelPosition(clickedCreaturePos->first, clickedCreaturePos->second);

        sf::RectangleShape panel(panelSize);
        panel.setFillColor(sf::Color(50, 50, 50, 205));
        panel.setOutlineThickness(2.0f);
        panel.setOutlineColor(sf::Color::Black);
        panel.setPosition(panelPosition);

        sf::Text infoText;

        // Set the font for the text
        infoText.setFont(font_);

        // Using the creature information stored in the creatureInfo member
        // Assuming creatureInfo is updated in the mousePressEvent method
        infoText.setString(creatureInfo.toStdString()); // Convert QString to std::string
        infoText.setCharacterSize(15);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(panelPosition.x + 10, panelPosition.y + 10);

        draw(panel);
        draw(infoText);
    } else if (showInfoPanel) {
        std::cout << "Info panel flag is set, but no creature position is recorded." << std::endl;
    } else {
        // Any other logic when the info panel is not shown
    }
}


sf::VertexArray createGradientCircle(float radius, const sf::Color& centerColor, const sf::Color& edgeColor) {
    const int points = 100;
    sf::VertexArray circle(sf::TriangleFan, points + 2);

           // Set the center of the circle at the origin (0, 0)
    circle[0].position = sf::Vector2f(0, 0);
    circle[0].color = centerColor;

    for (int i = 1; i <= points + 1; ++i) {
      float angle = (i - 1) * (2 * M_PI / points);
      // The points are now calculated from the origin
      sf::Vector2f point(radius * cos(angle), radius * sin(angle));

      float dist = std::sqrt(point.x * point.x + point.y * point.y) / radius;
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
        int generation = creature.GetGeneration();
        sf::Color color(20 + generation*20, 77, 64);
        sf::VertexArray creatureShape = createGradientCircle(creature.GetSize(), color, sf::Color(41, 40, 38));

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

void SimulationCanvas::mousePressEvent(QMouseEvent* event) {
    sf::Vector2f mousePos = mapPixelToCoords(sf::Vector2i(event->pos().x(), event->pos().y()));

    for (const auto& creature : simulation_->GetSimulationData()->creatures_) {
        auto [x, y] = creature.GetCoordinates();
        sf::Vector2f creaturePos(x, y);
        if (sqrt(pow(mousePos.x - creaturePos.x, 2) + pow(mousePos.y - creaturePos.y, 2)) <= 20) { // Assuming a creature radius of 10
            showInfoPanel = true;
            clickedCreaturePos = std::make_pair(mousePos.x, mousePos.y);
            creatureInfo = QString::fromStdString(formatCreatureInfo(creature));
            repaint();
            return;
        }
    }
    showInfoPanel = false;
}


bool SimulationCanvas::isCreatureClicked(const sf::Vector2f& mousePos) {
    for (const auto& creature : simulation_->GetSimulationData()->creatures_) {
        auto [x, y] = creature.GetCoordinates();
        sf::Vector2f creaturePos(x, y);
        if (sqrt(pow(mousePos.x - creaturePos.x, 2) + pow(mousePos.y - creaturePos.y, 2)) <= 20) { // Assuming a creature radius of 10
            return true;
        }
    }
    return false;
}

void SimulationCanvas::displayInfoPanel() {
    if (clickedCreaturePos) {
        showInfoPanel = true;
        panelRect = QRectF(clickedCreaturePos->first, clickedCreaturePos->second, 200, 100);
    }
}

std::string SimulationCanvas::formatCreatureInfo(const Creature& creature) {
    std::stringstream ss;
    ss << "Creature\n";
    ss << "Size: " << creature.GetSize() << "\n"; // Assuming Creature class has GetSize method
    ss << "Health: " << creature.GetHealth() << "\n"; // Assuming Creature class has GetHealth method
    ss << "Energy Level: " << creature.GetEnergy() << "\n"; // Assuming Creature class has GetEnergyLevel method
    return ss.str();
}
