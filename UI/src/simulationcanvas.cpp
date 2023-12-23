#include "simulationcanvas.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QUuid>
#include "creaturetracker.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <iostream>
#include <sstream>

SimulationCanvas::SimulationCanvas(QWidget *Parent)
    : QSFMLCanvas(Parent)
    , showInfoPanel(false)
{
    render_lambda_ = [this](SimulationData *data) { this->RenderSimulation(data); };
sf::RenderWindow& SimulationCanvas::GetRenderWindow() {
    return renderWindow_; // Use the correct member variable name
}



    QFile resourceFile(":/font.ttf");
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open font resource!";
        return;
    }

    // Generate a unique temporary filename
    QString tempFileName = "temp_font_" + QUuid::createUuid().toString(QUuid::WithoutBraces)
                           + ".ttf";
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

    trackButton_.setSize(sf::Vector2f(100, 30));         // Example size
    trackButton_.setFillColor(sf::Color(100, 100, 200)); // Example color
    trackButtonText_.setFont(font_);                     // Assuming you've already loaded a font
    trackButtonText_.setString("Track");
    trackButtonText_.setCharacterSize(15);
    trackButtonText_.setFillColor(sf::Color::White);

    // Clean up the temporary file after use
    QFile::remove(tempFilePath);
}

void SimulationCanvas::SetSimulation(Simulation* simulation) {
  simulation_ = simulation;
}

Simulation* SimulationCanvas::GetSimulation() { return simulation_; }

// called when the widget is first rendered
void SimulationCanvas::OnInit()
{
  clear(sf::Color(0, 255, 0));
}

void SimulationCanvas::OnUpdate()
{
  simulation_->ProcessData(render_lambda_);
  RenderSimulation(simulation_->GetSimulationData());

  // Check if a creature is selected and draw the red border if true
  if (showInfoPanel && selectedCreatureInfo) {
    // Right info panel setup
    sf::Vector2f panelSize(200, getSize().y);  // Width of 200 and full height of the canvas
    sf::Vector2f panelPosition(getSize().x - panelSize.x, 0);  // Positioned on the right side

    // Info panel background
    sf::RectangleShape panel(panelSize);
    panel.setFillColor(sf::Color(50, 50, 20, 205));  // Semi-transparent
    panel.setPosition(panelPosition);
    draw(panel);

    trackButton_.setPosition(panelPosition.x + 10, panelPosition.y + panelSize.y - 40);
    trackButtonText_.setPosition(trackButton_.getPosition().x + 10, trackButton_.getPosition().y + 5);

    // Draw the track button

    draw(trackButton_);
    draw(trackButtonText_);


    const auto& creatures = simulation_->GetSimulationData()->creatures_;
    auto creature_it = std::find_if(creatures.begin(), creatures.end(),
                                    [this](const Creature& c) {
                                        return c.GetID() == selectedCreatureInfo->id;
                                    });
    auto it = std::find_if(creatures.begin(), creatures.end(), [this](const Creature& c) {
        return c.GetID() == selectedCreatureInfo->id;
    });

    if (creature_it != creatures.end()) {
      const Creature& creature = *it;
      creatureInfo = QString::fromStdString(formatCreatureInfo(creature));

      if (selectedCreatureInfo && creature.GetID() == selectedCreatureInfo->id) {
          sf::CircleShape redCircle(creature.GetSize()); // Adjust as needed
          redCircle.setOutlineColor(sf::Color::Red);
          redCircle.setOutlineThickness(2); // Adjust thickness as needed
          redCircle.setFillColor(sf::Color::Transparent);
          redCircle.setPosition(creature.GetCoordinates().first - creature.GetSize(),
                                creature.GetCoordinates().second - creature.GetSize());
          draw(redCircle);

          // Check if the creature's health is 0 and display the message
          if (creature.GetHealth() == 0) {
              creatureInfo = QString::fromStdString("Creature " + std::to_string(creature.GetID()) + " is dead");
          } else {
              // Update the creature info normally
              creatureInfo = QString::fromStdString(formatCreatureInfo(creature));
          }
      }

      // Prepare and draw the creature info text inside the panel
      sf::Text infoText;
      infoText.setFont(font_);
      infoText.setString(creatureInfo.toStdString());
      infoText.setCharacterSize(15);
      infoText.setFillColor(sf::Color::White);
      infoText.setPosition(panelPosition.x + 10, 10);  // Adjust the Y position as needed
      draw(infoText);
    }
  }else if (showInfoPanel) {
    std::cout << "Info panel flag is set, but no creature position is recorded."
              << std::endl;
  }

  sf::Vector2i mousePixelPos = sf::Mouse::getPosition(*this);
  sf::Vector2f mousePos = mapPixelToCoords(mousePixelPos);

  // Prepare the text to display the mouse coordinates
  sf::Text mouseCoordsText;
  mouseCoordsText.setFont(font_);
  std::ostringstream ss;
  ss << "(X: " << mousePos.x << " Y: " << mousePos.y << ")";
  mouseCoordsText.setString(ss.str());
  mouseCoordsText.setCharacterSize(12);
  mouseCoordsText.setFillColor(sf::Color::White);

  // Position the text on the bottom right corner
  sf::FloatRect textRect = mouseCoordsText.getLocalBounds();
  mouseCoordsText.setOrigin(textRect.left + textRect.width, textRect.top + textRect.height);
  mouseCoordsText.setPosition(getSize().x - 10, getSize().y - 5);

  // Draw the mouse coordinates text
  draw(mouseCoordsText);
}

sf::VertexArray createGradientCircle(float radius, const sf::Color& centerColor,
                                     const sf::Color& edgeColor) {
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
        static_cast<sf::Uint8>(centerColor.b * (1 - dist) +
                               edgeColor.b * dist));

    circle[i].position = point;
    circle[i].color = color;
  }

  return circle;
}

// use this to process the simulation data and render it on the screen
void SimulationCanvas::RenderSimulation(SimulationData* data) {
  clear(sf::Color(9, 109, 6));

  // Iterate through food and create a gradient circle shape for each
  for (const auto& food : data->food_entities_) {
    sf::VertexArray foodShape = createGradientCircle(
        food.GetSize(), sf::Color(50, 100, 49), sf::Color(150, 250, 148));

    std::pair<double, double> foodCoordinates = food.GetCoordinates();
    sf::Transform foodTransform;
    foodTransform.translate(foodCoordinates.first, foodCoordinates.second);
    draw(foodShape, foodTransform);
  }

  // Iterate through creatures and create a gradient circle shape for each
  for (const auto& creature : data->creatures_) {
    int generation = creature.GetGeneration();
    sf::Color color(20 + generation * 20, 77, 64);
    sf::VertexArray creatureShape =
        createGradientCircle(creature.GetSize(), color, sf::Color(41, 40, 38));

    std::pair<double, double> creatureCoordinates = creature.GetCoordinates();
    sf::Transform creatureTransform;
    creatureTransform.translate(creatureCoordinates.first,
                                creatureCoordinates.second);
    draw(creatureShape, creatureTransform);
  }
}

void SimulationCanvas::DrawGraph(sf::RenderWindow& window,
                                 double (*graphFunction)(double), double xMin,
                                 double xMax, double yMin, double yMax) {
  // Set the coordinate system to match the window dimensions
  window.setView(
      sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));

  // Render the graph within the box
  sf::VertexArray line(sf::LinesStrip);
  double scaleFactor = 0.6;  // Adjust as needed

  // Calculate the starting x-coordinate to center the graph
  double startX = (window.getSize().x - scaleFactor * window.getSize().x) / 2;

  // Calculate the starting y-coordinate to center the graph vertically within
  // the box
  double startY = (window.getSize().y - scaleFactor * window.getSize().y) / 2;

  // Calculate the x-increment based on the range and resolution
  double xIncrement = (xMax - xMin) / (scaleFactor * window.getSize().x);

  for (double x = xMin; x <= xMax; x += xIncrement) {
    double y = graphFunction(x);

    // Map graph coordinates to window coordinates
    double mappedX =
        startX + (x - xMin) / (xMax - xMin) * scaleFactor * window.getSize().x;
    double mappedY =
        startY + (y - yMin) / (yMax - yMin) * scaleFactor * window.getSize().y;

    // Draw points
    line.append(sf::Vertex(sf::Vector2f(mappedX, mappedY), sf::Color::Blue));
  }

  // Draw a square black border around the graph area
  sf::RectangleShape borderRect;
  borderRect.setSize(sf::Vector2f(scaleFactor * window.getSize().x,
                                  scaleFactor * window.getSize().y));
  borderRect.setOutlineColor(sf::Color::Black);
  borderRect.setOutlineThickness(2.0);
  borderRect.setPosition(sf::Vector2f(startX, startY));  // Adjust as needed
  window.draw(borderRect);

  // Draw the graph
  window.draw(line);
}

void SimulationCanvas::DrawCreatureCountOverTime(
    sf::RenderWindow& window, const std::vector<Creature>& creatures) {
  // Set the coordinate system to match the window dimensions
  window.setView(
      sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));

  // Render the creature count over time graph within the box
  sf::VertexArray line(sf::LinesStrip);
  double scaleFactor = 0.6;  // Adjust as needed

  // Calculate the starting x-coordinate to center the graph
  double startX = (window.getSize().x - scaleFactor * window.getSize().x) / 2;

  // Calculate the starting y-coordinate to center the graph vertically within
  // the box
  double startY = (window.getSize().y - scaleFactor * window.getSize().y) / 2;

  // Declaration for left ticks
  sf::VertexArray leftTicks(sf::Lines);

  // Declaration for bottom ticks
  sf::VertexArray bottomTicks(sf::Lines);

  // Calculate the x-increment based on the range and resolution
  double xIncrement = 1.0;  // Assuming a time step of 1, adjust as needed

  for (std::size_t i = 0; i < creatures.size(); ++i) {
    // Use creatures.size() directly to get the count
    double creatureCount = creatures.size();

    // Map graph coordinates to window coordinates
    double mappedX = startX + i * xIncrement / creatures.size() * scaleFactor *
                                  window.getSize().x;
    double mappedY = startY + (creatureCount - 0.0) / (creatures.size() - 0.0) *
                                  scaleFactor * window.getSize().y;

    // Draw points
    line.append(sf::Vertex(sf::Vector2f(mappedX, mappedY), sf::Color::Blue));
  }

  // Draw a square black border around the graph area
  sf::RectangleShape borderRect;
  borderRect.setSize(sf::Vector2f(scaleFactor * window.getSize().x,
                                  scaleFactor * window.getSize().y));
  borderRect.setOutlineColor(sf::Color::Black);
  borderRect.setOutlineThickness(2.0);
  borderRect.setPosition(sf::Vector2f(startX, startY));  // Adjust as needed
  window.draw(borderRect);

  int numTicks = 5;         // Adjust as needed
  double tickLength = 5.0;  // Adjust as needed

  // Draw ticks on the left and add labels
  for (double y = startY; y <= startY + scaleFactor * window.getSize().y;
       y += scaleFactor * window.getSize().y / numTicks) {
    leftTicks.append(
        sf::Vertex(sf::Vector2f(startX - tickLength, y), sf::Color::Black));
    leftTicks.append(sf::Vertex(sf::Vector2f(startX, y), sf::Color::Black));

    // Add labels
    sf::Text label;
    label.setCharacterSize(12);
    label.setFillColor(sf::Color::Black);
    label.setString(std::to_string(static_cast<int>(
        (y - startY) / (scaleFactor * window.getSize().y) * creatures.size())));
    label.setPosition(startX - tickLength - 30, y - 6);  // Adjust as needed
    window.draw(label);
  }

  // Draw ticks on the bottom and add labels
  for (double x = startX; x <= startX + scaleFactor * window.getSize().x;
       x += scaleFactor * window.getSize().x / numTicks) {
    bottomTicks.append(
        sf::Vertex(sf::Vector2f(x, startY + scaleFactor * window.getSize().y),
                   sf::Color::Black));
    bottomTicks.append(sf::Vertex(
        sf::Vector2f(x, startY + scaleFactor * window.getSize().y + tickLength),
        sf::Color::Black));

    // Add labels
    sf::Text label;
    label.setCharacterSize(12);
    label.setFillColor(sf::Color::Black);
    label.setString(std::to_string(static_cast<int>(
        (x - startX) / (scaleFactor * window.getSize().x) * creatures.size())));
    label.setPosition(x - 15,
                      startY + scaleFactor * window.getSize().y + tickLength);
    window.draw(label);
  }

  // Draw the ticks
  window.draw(leftTicks);
  window.draw(bottomTicks);

  // Draw the graph
  window.draw(line);
}

void SimulationCanvas::mousePressEvent(QMouseEvent* event) {
  // Adjust coordinates for screen scaling
  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio from the QWidget

  // Convert QPoint to sf::Vector2i and adjust for the scaling factor
  sf::Vector2i scaledPos(static_cast<int>(event->pos().x() * scaleFactor),
                         static_cast<int>(event->pos().y() * scaleFactor));

  // Map the scaled pixel coordinates to world coordinates
  sf::Vector2f mousePos = mapPixelToCoords(scaledPos);

  qDebug() << "Mouse Pressed at: " << mousePos.x << ", " << mousePos.y;

  if (showInfoPanel) {
    auto bounds = trackButton_.getGlobalBounds();
    qDebug() << "Track Button Bounds: " << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height;
  }

  if (showInfoPanel && trackButton_.getGlobalBounds().contains(mousePos)) {
    qDebug() << "Track Button Clicked";
    if (selectedCreatureInfo) {
      const auto& creatures = simulation_->GetSimulationData()->creatures_;
      auto it = std::find_if(creatures.begin(), creatures.end(), [this](const Creature& c) {
          return c.GetID() == selectedCreatureInfo->id;
      });

      if (it != creatures.end()) {
          const Creature& selectedCreature = *it;
          CreatureTracker tracker(selectedCreature);
          tracker.Show();
      }
    }
  }

  for (const auto& creature : simulation_->GetSimulationData()->creatures_) {
    auto [creatureX, creatureY] = creature.GetCoordinates();
    float creatureSize = creature.GetSize();
    sf::Vector2f creaturePos(creatureX, creatureY);

    if (sqrt(pow(mousePos.x - creaturePos.x, 2) + pow(mousePos.y - creaturePos.y, 2)) <= creatureSize) {
      qDebug() << "Creature Clicked: ID" << creature.GetID();
      showInfoPanel = true;
      selectedCreatureInfo = CreatureInfo{creature.GetID(), creatureX, creatureY, creatureSize};
      repaint();
      return;
    }
  }

  qDebug() << "Click Outside, closing panel";
  showInfoPanel = false;
  selectedCreatureInfo.reset();
  repaint();
}


// Functions checking if a creature has been clicked
bool SimulationCanvas::isCreatureClicked(const sf::Vector2f& mousePos) {
  for (const auto& creature : simulation_->GetSimulationData()->creatures_) {
    auto [x, y] = creature.GetCoordinates();
    sf::Vector2f creaturePos(x, y);
    if (sqrt(pow(mousePos.x - creaturePos.x, 2) +
             pow(mousePos.y - creaturePos.y, 2)) <=
        creature.GetSize()+1) {
      return true;
    }
  }
  return false;
}

void SimulationCanvas::displayInfoPanel() {
  if (clickedCreaturePos) {
    showInfoPanel = true;
    panelRect =
        QRectF(clickedCreaturePos->first, clickedCreaturePos->second, 200, 100);
  }
}

// Structure of the info panel (appearing when a creature is clicked)
std::string SimulationCanvas::formatCreatureInfo(const Creature& creature) {
  std::stringstream ss;
  ss << "Creature ID: " << creature.GetID() << "\n\n";
  ss << "Size: " << creature.GetSize() << "\n";
  ss << "Age: " << creature.GetAge() << "\n";
  ss << "Generation: " << creature.GetGeneration() << "\n";
  ss << "Health: " << creature.GetHealth() << "\n";
  ss << "Energy Level: " << creature.GetEnergy() << "\n";
  ss << "Velocity: " << creature.GetVelocity() << "\n\n";
  auto [x, y] = creature.GetCoordinates();
  ss << "(x=" << x << ", y=" << y << ")\n";
  return ss.str();
}
