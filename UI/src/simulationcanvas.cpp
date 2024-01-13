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


SimulationCanvas::SimulationCanvas(QWidget* Parent)
    : QSFMLCanvas(Parent), showInfoPanel(false) {
  //Load textures for the sprites
  QPixmap pixmap;
  if (!pixmap.load(":/Resources/Pause.png")) {
    qDebug() << "Failed to load QPixmap from path:" << ":/Resources/Pause.png";
  }

  QImage qImage = pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
  sf::Image sfImage;
  sfImage.create(qImage.width(), qImage.height(), reinterpret_cast<const sf::Uint8*>(qImage.bits()));

  if (!texture_.loadFromImage(sfImage)) {
    qDebug() << "Failed to create sf::Texture from sf::Image";
  }

  scale_x_ = 1.0/texture_.getSize().x;
  scale_y_ = 1.0/texture_.getSize().y;


  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio

  // Scale the map width and height according to the pixel ratio
  int scaledWidth = static_cast<int>(width/scaleFactor);
  int scaledHeight = static_cast<int>(height/scaleFactor);

  // Set the fixed size with the scaled dimensions
  setFixedSize(scaledWidth, scaledHeight);

  InitializeFile(font_, ":/Resources/font.ttf");
  InitializeFile(color_shader_, ":/Shaders/colorShift.frag");
  InitializeFile(food_density_shader_, ":/Shaders/densityShader.frag");
  InitializeFile(creature_texture_, ":/Resources/Creature_base.png");
  InitializeFile(eyes_texture_, ":/Resources/Creature_eyes.png");
  InitializeFile(tail_texture_, ":/Resources/Creature_tails.png");
  InitializeFile(food_texture_, ":/Resources/Food_32x32.png");

  trackButton_.setSize(sf::Vector2f(100, 30));         // Example size
  trackButton_.setFillColor(sf::Color(100, 100, 200)); // Example color
  trackButtonText_.setFont(font_);                     // Assuming you've already loaded a font
  trackButtonText_.setString("Track");
  trackButtonText_.setCharacterSize(15);
  trackButtonText_.setFillColor(sf::Color::White);

  std::cout << "SimulationCanvas created" << std::endl;
}

void SimulationCanvas::InitializeFile(sf::Shader& ValueSaved, std::string path){
    QString qPath = QString::fromStdString(path);
    QFile resourceFile(qPath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
      qDebug() << "Failed to open resource!";
      return;
    }

    QString tempFileName = "temp_" + QUuid::createUuid().toString(QUuid::WithoutBraces)
            + ".frag";
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
      qDebug() << "Failed to copy to temporary file!";
      qDebug() << "Error:" << resourceFile.errorString();
      return;
    }
    resourceFile.close();

    if (!ValueSaved.loadFromFile(tempFilePath.toStdString(), sf::Shader::Fragment)) {
        qDebug() << "Failed to load from file!";
        QFile::remove(tempFilePath);
        return;
    }


    // Clean up the temporary file after use
    QFile::remove(tempFilePath);
}

void SimulationCanvas::InitializeFile(sf::Font& ValueSaved, std::string path){
    QString qPath = QString::fromStdString(path);
    QFile resourceFile(qPath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
      qDebug() << "Failed to open resource!";
      return;
    }

    QString tempFileName = "temp_" + QUuid::createUuid().toString(QUuid::WithoutBraces)
            + ".frag";
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
      qDebug() << "Failed to copy to temporary file!";
      qDebug() << "Error:" << resourceFile.errorString();
      return;
    }
    resourceFile.close();

    if (!ValueSaved.loadFromFile(tempFilePath.toStdString())){
        qDebug() << "Failed to load from file!";
        QFile::remove(tempFilePath);
        return;
    }


    // Clean up the temporary file after use
    QFile::remove(tempFilePath);
}

void SimulationCanvas::InitializeFile(sf::Texture& ValueSaved, std::string path){
    QPixmap creaturePixmap;
    QString qPath = QString::fromStdString(path);
    if (!creaturePixmap.load(qPath)) {
      //qDebug() << "Failed to load QPixmap from path:" << path;
    }

    QImage creatureqImage = creaturePixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
    sf::Image creaturesfImage;
    creaturesfImage.create(creatureqImage.width(), creatureqImage.height(), reinterpret_cast<const sf::Uint8*>(creatureqImage.bits()));

    if (!ValueSaved.loadFromImage(creaturesfImage)) {
      qDebug() << "Failed to create sf::Texture from sf::Image";
    }
}

void SimulationCanvas::UpdateFoodDensityTexture(double width, double height){
    food_density_texture_.create(width, height);
    sf::Image densityImage;
    densityImage.create(width, height, sf::Color::Black);
    // Fill the image with the density data
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            // Get the density value from the function
            float densityValue = simulation_->GetSimulationData()->GetEnvironment().GetFoodDensity(x, y);

            // Normalize the density value to the range 0 - 255 for the red channel
            sf::Uint8 greenValue = static_cast<sf::Uint8>(255 * std::min(
                                                              std::max(densityValue, 0.0f)/settings::engine::kMaxFoodDensityColored,
                                                              1.0));

            // Set the pixel color in the image
            densityImage.setPixel(x, y, sf::Color(0, greenValue, 0));
        }
    }
    food_density_texture_.setSmooth(true);
    // Load the image into the texture
    food_density_texture_.update(densityImage);
}

void SimulationCanvas::SetSimulation(Simulation* simulation) {
  simulation_ = simulation;
}

Simulation* SimulationCanvas::GetSimulation() { return simulation_; }

// called when the widget is first rendered
void SimulationCanvas::OnInit()
{
  clear(sf::Color(0, 255, 0));
  initialViewCenter = getView().getCenter();
  initialViewSize = getView().getSize();
}

void SimulationCanvas::OnUpdate()
{
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

    //Get the selected creature and draw the red border
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

      // Draw the energy bar
      float maxBarWidth = 80.0f;  // Width of the full energy bar
      float barHeight = 10.0f;    // Height of the energy bar

      float energyRatio = static_cast<float>(creature.GetEnergy()) / static_cast<float>(creature.GetMaxEnergy());
      if (energyRatio < 0) {
          energyRatio = 0;
      }
      float energyBarWidth = maxBarWidth * energyRatio;  // Width based on current energy
      sf::RectangleShape energyBarOutline(sf::Vector2f(maxBarWidth, barHeight));
      sf::RectangleShape energyBar(sf::Vector2f(energyBarWidth, barHeight));
      energyBar.setFillColor(sf::Color::Green);  // Color of the energy bar
      energyBarOutline.setFillColor(sf::Color::Black);
      // Position the energy bar - adjust as needed
      energyBarOutline.setPosition(panelPosition.x + 40, 123);
      energyBar.setPosition(panelPosition.x + 40, 123);

      //Energy texture
      QPixmap energyPixmap;
      if (!energyPixmap.load(":/Resources/Energy.png")) {
          qDebug() << "Failed to load QPixmap from path:" << ":/Resources/Energy.png";
      }

      QImage energyqImage = energyPixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
      sf::Image energysfImage;
      energysfImage.create(energyqImage.width(), energyqImage.height(), reinterpret_cast<const sf::Uint8*>(energyqImage.bits()));


      if (!energy_texture_.loadFromImage(energysfImage)) {
          qDebug() << "Failed to create sf::Texture from sf::Image";
      }

      sf::Sprite energySprite;
      energySprite.setTexture(energy_texture_);
      energySprite.setScale(0.04f,0.04f);
      energySprite.setPosition(panelPosition.x + 10, 117);

      draw(energySprite);
      draw(energyBarOutline);
      draw(energyBar);

      // Draw the health bar

      float healthRatio = static_cast<float>(creature.GetHealth()) / static_cast<float>(creature.GetMutable().GetIntegrity() * pow(creature.GetSize(), 2));
      if (healthRatio < 0 ) {
          healthRatio = 0;
      }
      float healthBarWidth = maxBarWidth * healthRatio;  // Width based on current energy
      sf::RectangleShape healthBarOutline(sf::Vector2f(maxBarWidth, barHeight));
      sf::RectangleShape healthBar(sf::Vector2f(healthBarWidth, barHeight));
      healthBar.setFillColor(sf::Color::Red);  // Color of the energy bar
      healthBarOutline.setFillColor(sf::Color::Black);
      // Position the energy bar - adjust as needed
      healthBarOutline.setPosition(panelPosition.x + 40, 105);
      healthBar.setPosition(panelPosition.x + 40, 105);

      //Health texture
      QPixmap healthPixmap;
      if (!healthPixmap.load(":/Resources/Health.png")) {
          qDebug() << "Failed to load QPixmap from path:" << ":/Resources/Health.png";
      }

      QImage healthqImage = healthPixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
      sf::Image healthsfImage;
      healthsfImage.create(healthqImage.width(), healthqImage.height(), reinterpret_cast<const sf::Uint8*>(healthqImage.bits()));


      if (!health_texture_.loadFromImage(healthsfImage)) {
          qDebug() << "Failed to create sf::Texture from sf::Image";
      }

      sf::Sprite healthSprite;
      healthSprite.setTexture(health_texture_);
      healthSprite.setScale(0.7f,0.7f);
      healthSprite.setPosition(panelPosition.x + 10, 100);

      draw(healthSprite);
      draw(healthBarOutline);
      draw(healthBar);

      if (selectedCreatureInfo && creature.GetID() == selectedCreatureInfo->id) {

          sf::CircleShape redCircle(creature.GetSize()); // Adjust as needed
          redCircle.setOutlineColor(sf::Color::Red);
          redCircle.setOutlineThickness(creature.GetSize()/5); // Adjust thickness as needed
          redCircle.setFillColor(sf::Color::Transparent);
          redCircle.setPosition(creature.GetCoordinates().first - creature.GetSize(),
                                creature.GetCoordinates().second - creature.GetSize());
          draw(redCircle);

          DrawVisionCone(*this, creature);

          // Check if the creature's health is 0 and display the message
          if (creature.GetHealth() == 0) {
              creatureInfo = QString::fromStdString("Creature " + std::to_string(creature.GetID()) + " is dead");
          } else {
              // Update the creature info normally
              creatureInfo = QString::fromStdString(formatCreatureInfo(creature));
          }
      }
      //Get the closest food for the creature and draw a blue circle around it
      const auto& food_entities = simulation_->GetSimulationData()->food_entities_;
      auto food_it = std::find_if(food_entities.begin(), food_entities.end(),
                                      [this, creature](const Food& c) {
                                          return c.GetID() == creature.GetFoodID();
                                      });
      auto it = std::find_if(food_entities.begin(), food_entities.end(), [this, creature](const Food& c) {
          return c.GetID() == creature.GetFoodID();
      });

      if (food_it != food_entities.end()) {
        const Food& food = *it;

        if (selectedCreatureInfo && food.GetID() == creature.GetFoodID()) {

            sf::CircleShape blueCircle(food.GetSize()); // Adjust as needed
            blueCircle.setOutlineColor(sf::Color::Blue);
            blueCircle.setOutlineThickness(2); // Adjust thickness as needed
            blueCircle.setFillColor(sf::Color::Transparent);
            blueCircle.setPosition(food.GetCoordinates().first - food.GetSize(),
                                   food.GetCoordinates().second - food.GetSize());
            draw(blueCircle);
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
void SimulationCanvas::RenderSimulation(DataAccessor<SimulationData> data) {
  clear(sf::Color(20, 22, 69));
  /*
  // Create a full-screen rectangle
  double mapWidth = data->GetEnvironment().GetMapWidth();
  double mapHeight = data->GetEnvironment().GetMapHeight();
  // Apply the shader to the background
  sf::RenderStates backgroundState;

  sf::View currentView = getView();
  sf::Vector2f viewTopLeft = currentView.getCenter() - (currentView.getSize() / 2.0f);
  sf::Vector2f viewSize = currentView.getSize(); // View size considering the zoom

  food_density_shader_.setUniform("resolution", sf::Vector2f(mapWidth, mapHeight));
  food_density_shader_.setUniform("viewTopLeft", viewTopLeft);
  food_density_shader_.setUniform("viewSize", viewSize);  // Updated to pass view size
  food_density_shader_.setUniform("densityTexture", food_density_texture_);
  food_density_shader_.setUniform("maxDensity", 1.0f);

  backgroundState.shader = &food_density_shader_;

  for (int xShift = -1; xShift <= 1; xShift++) {
    for (int yShift = -1; yShift <= 1; yShift++) {
      sf::RectangleShape background(sf::Vector2f(mapWidth, mapHeight));
      background.setPosition(xShift * mapWidth, yShift * mapHeight);
      draw(background, backgroundState);
    }
  }
  */
  // Iterate through food and load the corresponding sprite
  // Note that we are assuming to be working with a sprite sheet of 256x256 per sprite

  for (const auto& food : data->food_entities_) {
    auto renderPositions = getEntityRenderPositions(food);
    for (const auto& pos : renderPositions) {
      RenderFoodAtPosition(food, pos);
    }
  }

  // Iterate through creatures and create a gradient circle shape for each
  for (const auto& creature : data->creatures_) {
    auto renderPositions = getEntityRenderPositions(creature);
    for (const auto& pos : renderPositions) {
      RenderCreatureAtPosition(creature, pos);
    }
  }
}

void SimulationCanvas::RenderFoodAtPosition(const Food& food, const std::pair<double, double>& position){
  sf::Sprite foodSprite;
  foodSprite.setTexture(food_texture_);
  int spriteIndex = food.GetID() % 3;
  if (food.GetType() == Food::type::plant) {
    foodSprite.setTextureRect(sf::IntRect(0, spriteIndex * 256, 256, 256));
  } else if (food.GetType() == Food::type::meat) {
    foodSprite.setTextureRect(sf::IntRect(256, spriteIndex * 256, 256, 256));
  }
  foodSprite.setOrigin(128.0f, 128.0f);

  foodSprite.setScale(food.GetSize()/128.0f, food.GetSize()/128.0f);

  sf::Transform foodTransform;
  foodTransform.translate(position.first, position.second);
  draw(foodSprite, foodTransform);
}

void SimulationCanvas::RenderCreatureAtPosition(const Creature& creature, const std::pair<double, double>& position){
  sf::Sprite base_sprite;
  sf::Sprite eyes_sprite;
  sf::Sprite tail_sprite;
  base_sprite.setTexture(creature_texture_);
  eyes_sprite.setTexture(eyes_texture_);
  tail_sprite.setTexture(tail_texture_);

  //Get which sprites to use depending on the characteristics of the creature
  int size_type  = std::floor((15 - creature.GetSize())/5); //size is the other way around
  size_type = size_type > 2 ? 2 : size_type;
  int eyes_type = std::floor(creature.GetMutable().GetVisionFactor()/100);
  eyes_type = eyes_type > 3 ? 3 : eyes_type;
  int tail_type = std::floor(creature.GetMutable().GetMaxForce()/5);
  tail_type = tail_type > 3 ? 3 : tail_type;

  //Assuming the sprites are size 768x768
  base_sprite.setTextureRect(sf::IntRect(0, size_type * 768, 768, 768));
  eyes_sprite.setTextureRect(sf::IntRect(eyes_type * 768, size_type * 768, 768, 768));
  tail_sprite.setTextureRect(sf::IntRect(tail_type * 768, size_type * 768, 768, 768));

  //To make the origin at 0 so that the creature rotates correctly
  base_sprite.setOrigin(384.0f, 384.0f);
  eyes_sprite.setOrigin(384.0f, 384.0f);
  tail_sprite.setOrigin(384.0f, 384.0f);

  //We use a certain proportion because the creature's body doesn't occupy the entirety of the image
  base_sprite.setScale(creature.GetSize()/208.0f, creature.GetSize()/208.0f);
  eyes_sprite.setScale(creature.GetSize()/208.0f, creature.GetSize()/208.0f);
  tail_sprite.setScale(creature.GetSize()/208.0f, creature.GetSize()/208.0f);

  //Rotation offset
  base_sprite.setRotation(90.0f);
  eyes_sprite.setRotation(90.0f);
  tail_sprite.setRotation(90.0f);

  // Use creature coordinates directly for position
  std::pair<double, double> creatureCoordinates = creature.GetCoordinates();

  //Add color filter to creature
  color_shader_.setUniform("hueShift", creature.GetMutable().GetColor());

  sf::Transform creatureTransform;
  creatureTransform.translate(position.first,
                              position.second);
  creatureTransform.rotate(creature.GetOrientation() * 180.0f /M_PI);

  sf::RenderStates states;
  states.shader = &color_shader_;
  states.transform = creatureTransform;

  draw(base_sprite, states);
  draw(eyes_sprite, states);
  draw(tail_sprite, states);
}


std::vector<std::pair<double, double>> SimulationCanvas::getEntityRenderPositions(const Entity& entity) {
    std::vector<std::pair<double, double>> positions;
    auto [entityX, entityY] = entity.GetCoordinates();
    double entitySize = entity.GetSize();
    sf::Vector2f viewCenter = getView().getCenter();
    sf::Vector2f viewSize = getView().getSize();
    double mapWidth = GetSimulation()->GetSimulationData()->GetEnvironment().GetMapWidth();
    double mapHeight = GetSimulation()->GetSimulationData()->GetEnvironment().GetMapHeight();

    // Function to check if position is within view bounds
    auto isInView = [&](double x, double y) {
        double halfWidth = viewSize.x / 2;
        double halfHeight = viewSize.y / 2;
        double leftBound = viewCenter.x - halfWidth - entitySize;
        double rightBound = viewCenter.x + halfWidth + entitySize;
        double topBound = viewCenter.y - halfHeight - entitySize;
        double bottomBound = viewCenter.y + halfHeight + entitySize;

        return (x + entitySize > leftBound) && (x - entitySize < rightBound) &&
               (y + entitySize > topBound) && (y - entitySize < bottomBound);
    };

    // Check original position
    if (isInView(entityX, entityY)) {
        positions.emplace_back(entityX, entityY);
    }

    // Check wrap-around positions
    std::vector<std::pair<double, double>> potentialPositions = {
        {entityX + mapWidth, entityY}, {entityX - mapWidth, entityY},
        {entityX, entityY + mapHeight}, {entityX, entityY - mapHeight},
        {entityX + mapWidth, entityY + mapHeight}, {entityX - mapWidth, entityY - mapHeight},
        {entityX + mapWidth, entityY - mapHeight}, {entityX - mapWidth, entityY + mapHeight}
    };

    for (const auto& pos : potentialPositions) {
        if (isInView(pos.first, pos.second)) {
            positions.push_back(pos);
        }
    }

    return positions;
}


void SimulationCanvas::mousePressEvent(QMouseEvent* event) {
  // Adjust coordinates for screen scaling
  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio from the QWidget

  // Convert QPoint to sf::Vector2i and adjust for the scaling factor
  sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                         static_cast<int>(event->position().y() * scaleFactor));

  // Map the scaled pixel coordinates to world coordinates
  sf::Vector2f mousePos = mapPixelToCoords(scaledPos);

  initialClickPosition = mousePos;
  isClicking = true;

  qDebug() << "Mouse Pressed at: " << mousePos.x << ", " << mousePos.y;

  if (showInfoPanel) {
    auto bounds = trackButton_.getGlobalBounds();
    qDebug() << "Track Button Bounds: " << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height;
  }

  if (showInfoPanel && trackButton_.getGlobalBounds().contains(mousePos)) {
    qDebug() << "Track Button Clicked";
    if (selectedCreatureInfo) {
      auto data = simulation_->GetSimulationData();
      const auto& creatures = data->creatures_;
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

  auto data = simulation_->GetSimulationData();

  for (const auto& creature : data->creatures_) {
    auto [creatureX, creatureY] = creature.GetCoordinates();
    float creatureSize = creature.GetSize();
    sf::Vector2f creaturePos(creatureX, creatureY);

    if (sqrt(pow(mousePos.x - creaturePos.x, 2) + pow(mousePos.y - creaturePos.y, 2)) <= creatureSize) {
      qDebug() << "Creature Clicked: ID" << creature.GetID();
      showInfoPanel = true;
      selectedCreatureInfo = CreatureInfo{creature.GetID(), creatureX, creatureY, creatureSize, creature.GetFoodID()};
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

double round_double(double number, int decimal_places) {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::round(number * multiplier) / multiplier;
}

// Structure of the info panel (appearing when a creature is clicked)
std::string SimulationCanvas::formatCreatureInfo(const Creature& creature) {
  Mutable mutables = creature.GetMutable();
  std::stringstream ss;
  ss << "Creature ID: " << creature.GetID() << "\n\n";
  ss << "Size: " << creature.GetSize() << "\n";
  ss << "Age: " << creature.GetAge() << "\n";
  ss << "Generation: " << creature.GetGeneration() << "\n";
  ss << "\n";
  ss << "\n";
  ss << "Velocity: " << round_double(creature.GetVelocity(), 2) << "\n";
  ss << "Rot. Velocity: " << round_double(creature.GetRotationalVelocity(), 2) << "\n\n";
  ss << "Stomach capacity: " << creature.GetStomachCapacity() << "\n\n";
  ss << "Stomach fullness: " << creature.GetStomachFullness() << "\n\n";
  ss << "Stomach acid: " << creature.GetAcid() << "\n\n";
  auto [x, y] = creature.GetCoordinates();
  ss << "(x=" << x << ", y=" << y << ")\n \n";
  ss << "Mutables information: \n";
  ss << "Energy density: " << mutables.GetEnergyDensity() << "\n";
  ss << "Energy loss: " << mutables.GetEnergyLoss() << "\n";
  ss << "Strafing diff.: " << mutables.GetStrafingDifficulty() << "\n";
  ss << "Integrity: " << mutables.GetIntegrity() << "\n";
  ss << "Max force: " << mutables.GetMaxForce() << "\n";
  ss << "Max size: " << mutables.GetMaxSize() << "\n";
  ss << "Baby size: " << mutables.GetBabySize() << "\n";
  ss << "Rep. cooldown: " << mutables.GetReproductionCooldown() << "\n";
  ss << "Maturity age: " << mutables.GetMaturityAge() << "\n";
  return ss.str();
}

void SimulationCanvas::DrawVisionCone(sf::RenderTarget& target, const Creature& creature) {

  double visionRadius = creature.GetVisionRadius();
  double visionAngle = creature.GetVisionAngle();

  double creatureOrientation = creature.GetOrientation();

  double leftRad = creatureOrientation - visionAngle / 2.0;
  double rightRad = creatureOrientation + visionAngle / 2.0;

  auto [creatureX, creatureY] = creature.GetCoordinates();

  std::vector<sf::Vertex> triangleFan;
  triangleFan.push_back(sf::Vertex(sf::Vector2f(creatureX, creatureY), sf::Color::Transparent));

  int numPoints = 30;
  for (int i = 0; i <= numPoints; ++i) {
    double angle = leftRad + (i * (rightRad - leftRad) / numPoints);
    double x = creatureX + visionRadius * cos(angle);
    double y = creatureY + visionRadius * sin(angle);
    triangleFan.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color(255, 255, 255, 150)));
  }

  for (size_t i = 1; i < triangleFan.size() - 1; ++i) {
    sf::VertexArray triangle(sf::Triangles, 3);
    triangle[0] = triangleFan[0];
    triangle[1] = triangleFan[i];
    triangle[2] = triangleFan[i + 1];
    target.draw(triangle);
  }
}

void SimulationCanvas::wheelEvent(QWheelEvent *event) {
    float delta = event->angleDelta().y() > 0 ? 1.1f : 0.9f;

    // Adjust coordinates for screen scaling
    float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio from the QWidget

    // Convert QPoint to sf::Vector2i and adjust for the scaling factor
    sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                           static_cast<int>(event->position().y() * scaleFactor));

    // Map the scaled pixel coordinates to world coordinates
    sf::Vector2f worldPos = mapPixelToCoords(scaledPos);
    zoom(delta, worldPos);
}

void SimulationCanvas::zoom(float factor, sf::Vector2f& zoomPoint) {
    //Get the current view
    sf::View view = getView();
    sf::Vector2f newCenter;
    // Check for maximum zoom
    if (zoomFactor * factor > 1) {
        zoomFactor = 1;
        newCenter = initialViewCenter;
    } else {
        zoomFactor *= factor;
        // Get the current view center
        sf::Vector2f currentCenter = view.getCenter();

        // Calculate the direction vector from current center to zoom point
        sf::Vector2f direction = currentCenter - zoomPoint;

        // Decide how much of this direction to apply
        // For a more noticeable zoom effect, you can adjust the multiplier (e.g., 0.1)
        sf::Vector2f movement = direction * (factor - 1.0f);

        // Calculate the new center
        newCenter = currentCenter + movement;
    }
    // Set the new center and size of the view
    view.setCenter(newCenter);
    view.setSize(initialViewSize.x * zoomFactor, initialViewSize.y * zoomFactor);

    // Apply the new view and update
    setView(view);
    update();
}

void SimulationCanvas::mouseMoveEvent(QMouseEvent* event) {
    float scaleFactor = this->devicePixelRatioF();
    sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                           static_cast<int>(event->position().y() * scaleFactor));
    sf::Vector2f currentMousePosition = mapPixelToCoords(scaledPos);

    if (isClicking) {
        float distance = sqrt(pow(currentMousePosition.x - initialClickPosition.x, 2) +
                              pow(currentMousePosition.y - initialClickPosition.y, 2));

        if (distance > SETTINGS.ui.dragging_sensitivity) {
      qDebug() << "Switched to dragging";
      isDragging = true;
      isClicking = false;
        }
    }
    if (isDragging) {
        sf::Vector2f delta = initialClickPosition - currentMousePosition;
        deltaHistory.push_back(delta);
        if (deltaHistory.size() > 5) {
            deltaHistory.pop_front();
        }

        sf::Vector2f averagedDelta = std::accumulate(deltaHistory.begin(), deltaHistory.end(), sf::Vector2f(0, 0));
        averagedDelta /= static_cast<float>(deltaHistory.size());

        sf::View view = getView();
        view.move(averagedDelta);
        // Get map dimensions
        double mapWidth = simulation_->GetSimulationData()->GetEnvironment().GetMapWidth();
        double mapHeight = simulation_->GetSimulationData()->GetEnvironment().GetMapHeight();

        // Get the new center of the view
        sf::Vector2f newCenter = view.getCenter();

        // Wrap around horizontally
        if (newCenter.x < 0) {
            currentMousePosition.x += mapWidth;
            newCenter.x += mapWidth;
        }
        else if (newCenter.x > mapWidth) {
            currentMousePosition.x -= mapWidth;
            newCenter.x -= mapWidth;
        }

        // Wrap around vertically
        if (newCenter.y < 0) {
            currentMousePosition.y += mapHeight;
            newCenter.y += mapHeight;
        }
        else if (newCenter.y > mapHeight) {
            currentMousePosition.y -= mapHeight;
            newCenter.y -= mapHeight;
        }
        // Set the wrapped center back to the view
        view.setCenter(newCenter);
        setView(view);

        initialClickPosition = currentMousePosition;
    }
}

void SimulationCanvas::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "Mouse released";
    if (isDragging) {
        isDragging = false;
    }
    isClicking = false;
}

void SimulationCanvas::resizeEvent(QResizeEvent* event) {
    sf::Vector2u newSize(event->size().width(), event->size().height());
    sf::View view = getView();
    view.setSize(newSize.x, newSize.y);
    view.setCenter(newSize.x / 2, newSize.y / 2);
    setView(view);
}
