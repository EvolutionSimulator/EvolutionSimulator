#include "simulationcanvas/simulationcanvas.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QUuid>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <iostream>
#include <sstream>

SimulationCanvas::SimulationCanvas(QWidget* Parent)
    : QSFMLCanvas(Parent),
      texture_manager_(TextureManager()),
      info_panel_(InfoPanel(this, &texture_manager_, simulation_)) {
  // Scale the map width and height according to the pixel ratio
  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  float scaleFactor = this->devicePixelRatioF();  // Get the device pixel ratio

  int scaledWidth = static_cast<int>(width / scaleFactor);
  int scaledHeight = static_cast<int>(height / scaleFactor);

  // Set the fixed size with the scaled dimensions
  setFixedSize(scaledWidth, scaledHeight);

  // Grab the pinch gesture for zooming
  grabGesture(Qt::PinchGesture);

  std::cout << "SimulationCanvas created" << std::endl;
}




void SimulationCanvas::UpdateFoodDensityTexture(SimulationData& data){
    double width = SETTINGS.environment.map_width;
    double height = SETTINGS.environment.map_height;

    texture_manager_.food_density_texture_.create(width, height);
    sf::Image densityImage;
    densityImage.create(width, height, sf::Color::Black);
    // Fill the image with the density data
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            // Get the density value from the function
            float densityValue = data.GetEnvironment().GetFoodDensity(x, y);

            // Normalize the density value to the range 0 - 255 for the red
            // channel
            sf::Uint8 greenValue = static_cast<sf::Uint8>(
                255 * std::min(std::max(densityValue, 0.0f) /
                                   SETTINGS.engine.max_food_density_colored,
                               1.0));

            // Set the pixel color in the image
            densityImage.setPixel(x, y, sf::Color(0, greenValue, 0));
    }
  }
  texture_manager_.food_density_texture_.setSmooth(true);
  // Load the image into the texture
  texture_manager_.food_density_texture_.update(densityImage);
}

void SimulationCanvas::SetSimulation(Simulation* simulation) {
  simulation_ = simulation;
}

Simulation* SimulationCanvas::GetSimulation() { return simulation_; }

// called when the widget is first rendered
void SimulationCanvas::OnInit() {
  clear(sf::Color(0, 255, 0));
  initialViewCenter = getView().getCenter();
  initialViewSize = getView().getSize();

  ui_view_ = getView();
  info_panel_view_ = getView();

  OnUpdate();
}

void SimulationCanvas::OnUpdate()
{
  setView(ui_view_);

  if (followCreature && followedCreature) {
          // Update the view to follow the creature
          sf::Vector2f currPos(followedCreature->GetCoordinates().first, followedCreature->GetCoordinates().second);
          //creatureViewHistory.push_back(currPos);
          centerViewAroundCreature({static_cast<float>(followedCreature->GetCoordinates().first), static_cast<float>(followedCreature->GetCoordinates().second)});
          currTopLeft = ui_view_.getCenter() - sf::Vector2f(ui_view_.getSize().x / 2, ui_view_.getSize().x / 2);
      }

  DataAccessor<SimulationData> data = simulation_->GetSimulationData();
  SimulationData& data_ref = *data;
  RenderSimulation(data_ref);

  // Check if a creature is selected
  DrawMouseCoordinates();
  if (info_panel_.IsVisible() && info_panel_.GetSelectedCreature()) {
    if (info_panel_.GetSelectedCreature()->GetState() == AliveEntity::Alive)
    {
      info_panel_.UpdateSelectedFood();
      info_panel_.SetUIView(ui_view_);
      info_panel_.SetPanelView(info_panel_view_);
      info_panel_.Draw();
    }
  } else if (info_panel_.IsVisible()) {
    std::cout << "Info panel flag is set, but no creature position is recorded."
              << std::endl;
  }
  setView(ui_view_);
}

bool SimulationCanvas::gestureEvent(QGestureEvent* event) {
  if (QGesture* pinch = event->gesture(Qt::PinchGesture))
    pinchTriggered(static_cast<QPinchGesture*>(pinch));
  return true;
}

void SimulationCanvas::pinchTriggered(QPinchGesture* gesture) {
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  if (changeFlags & QPinchGesture::ScaleFactorChanged) {
    float delta = gesture->scaleFactor();
    zoom(1 / delta);
    qDebug() << "pinchTriggered(): zoom by" << delta;
  }
}

void SimulationCanvas::DrawMouseCoordinates() {
  sf::Vector2i mousePixelPos = sf::Mouse::getPosition(*this);
  sf::Vector2f mousePos = mapPixelToCoords(mousePixelPos);

  mousePos.x = fmod(mousePos.x + static_cast<float>(SETTINGS.environment.map_width), static_cast<float>(SETTINGS.environment.map_width));
  mousePos.y = fmod(mousePos.y + static_cast<float>(SETTINGS.environment.map_height), static_cast<float>(SETTINGS.environment.map_height));

  // Prepare the text to display the mouse coordinates
  sf::Text mouseCoordsText;
  mouseCoordsText.setFont(texture_manager_.font_);
  std::ostringstream ss;
  ss << "(X: " << mousePos.x << " Y: " << mousePos.y << ")";
  mouseCoordsText.setString(ss.str());
  mouseCoordsText.setCharacterSize(12);
  mouseCoordsText.setFillColor(sf::Color::White);

  setView(info_panel_view_);

  // Estimate the size of the text box (e.g., 140x18 pixels)
  int estimatedTextWidth = 140; // This is an example, adjust based on your text
  int estimatedTextHeight = 20; // Adjust based on your font and character size

  // Position the text on the bottom right corner of the info_panel_view
  float textX = getSize().x - estimatedTextWidth - 10; // 10 pixels padding from the right edge
  float textY = getSize().y - estimatedTextHeight - 5; // 5 pixels padding from the bottom edge

  mouseCoordsText.setPosition(textX, textY);
  draw(mouseCoordsText);
}

// use this to process the simulation data and render it on the screen
void SimulationCanvas::RenderSimulation(SimulationData& data) {
  clear(sf::Color(20, 22, 69));

  // Iterate through food and load the corresponding sprite
  // Note that we are assuming to be working with a sprite sheet of 256x256 per
  // sprite

  for (const auto& food_ptr : data.food_entities_) {
    if (food_ptr == nullptr) continue;
    std::shared_ptr<Food> food = food_ptr;
    auto renderPositions = getEntityRenderPositions(food);
    for (const auto& pos : renderPositions) {
      RenderFoodAtPosition(food, pos);
    }
  }

  for (const auto& egg_ptr : data.eggs_) {
    if (egg_ptr == nullptr) continue;
    std::shared_ptr<Egg> egg = egg_ptr;
    auto renderPositions = getEntityRenderPositions(egg);  // QUICK FIX - NEED FOOD TO INHERIT VIRTUALLY FROM ENTITY
    for (const auto& pos : renderPositions) {
      RenderEggAtPosition(egg, pos);
    }
  }

  int size = data.creatures_.size();
  // Iterate through creatures and create a gradient circle shape for each
  for (const auto& creature_ptr : data.creatures_) {
    if (creature_ptr == nullptr) continue;
    auto& creature_ref = *creature_ptr;
    int id = creature_ref.GetID(); //Not sure but I think this line and the above aren't used
    std::shared_ptr<Creature> creature = creature_ptr;
    auto renderPositions = getEntityRenderPositions(creature);
    for (const auto& pos : renderPositions) {
      RenderCreatureAtPosition(creature, pos);
    }
  }

  for (const auto& pheromone : data.pheromones_) {
      auto renderPositions = getEntityRenderPositions(pheromone);
      for (const auto& pos : renderPositions) {
          RenderPheromoneAtPosition(pheromone, pos);
      }
  }
}

void SimulationCanvas::RenderFoodAtPosition(const std::shared_ptr<Food> food, const std::pair<double, double>& position){
  if (food == nullptr) return;
  sf::Sprite foodSprite;
  foodSprite.setTexture(texture_manager_.food_texture_);
  int spriteIndex = food->GetID() % 3;
  if (food->GetType() == Food::type::plant) {
    foodSprite.setTextureRect(sf::IntRect(0, spriteIndex * 256, 256, 256));
  } else if (food->GetType() == Food::type::meat) {
    foodSprite.setTextureRect(sf::IntRect(256, spriteIndex * 256, 256, 256));
  }
  foodSprite.setOrigin(128.0f, 128.0f);

  foodSprite.setScale(food->GetSize()/128.0f, food->GetSize()/128.0f);

  texture_manager_.color_shader_.setUniform("hueShift", food->GetColor());

  sf::Transform foodTransform;
  foodTransform.translate(position.first, position.second);

  sf::RenderStates states;
  states.shader = &texture_manager_.color_shader_;
  states.transform = foodTransform;

  draw(foodSprite, states);
}

void SimulationCanvas::RenderEggAtPosition(
    std::shared_ptr<Egg> egg, const std::pair<double, double>& position) {
  sf::Sprite eggSprite;
  eggSprite.setTexture(texture_manager_.egg_texture_);
  eggSprite.setOrigin(160.0f, 160.0f);
  eggSprite.setScale(egg->GetSize()/160.0f , egg->GetSize()/160.0f);

  texture_manager_.color_shader_.setUniform("hueShift", egg->GetMutable().GetColor());

  std::pair<double, double> eggCoordinates = egg->GetCoordinates();
  sf::Transform eggTransform;
  eggTransform.translate(eggCoordinates.first, eggCoordinates.second);

  sf::RenderStates states;
  states.shader = &texture_manager_.color_shader_;
  states.transform = eggTransform;

  draw(eggSprite, states);
}

void SimulationCanvas::RenderCreatureAtPosition(
    const std::shared_ptr<Creature> creature, const std::pair<double, double>& position) {
  if (creature == nullptr) return;
  sf::Sprite base_sprite;
  sf::Sprite eyes_sprite;
  sf::Sprite tail_sprite;
  base_sprite.setTexture(texture_manager_.creature_texture_);
  eyes_sprite.setTexture(texture_manager_.eyes_texture_);
  tail_sprite.setTexture(texture_manager_.tail_texture_);

  //Get which sprites to use depending on the characteristics of the creature
  int size_type  = std::floor((15 - creature->GetSize())/5); //size is the other way around
  size_type = size_type > 2 ? 2 : size_type;
  int eyes_type = std::floor(creature->GetMutable().GetVisionFactor()/100);
  eyes_type = eyes_type > 3 ? 3 : eyes_type;
  int tail_type = std::floor(creature->GetMutable().GetMaxForce()/5);
  tail_type = tail_type > 3 ? 3 : tail_type;

  // Assuming the sprites are size 768x768
  base_sprite.setTextureRect(sf::IntRect(0, size_type * 768, 768, 768));
  eyes_sprite.setTextureRect(
      sf::IntRect(eyes_type * 768, size_type * 768, 768, 768));
  tail_sprite.setTextureRect(
      sf::IntRect(tail_type * 768, size_type * 768, 768, 768));

  // To make the origin at 0 so that the creature rotates correctly
  base_sprite.setOrigin(384.0f, 384.0f);
  eyes_sprite.setOrigin(384.0f, 384.0f);
  tail_sprite.setOrigin(384.0f, 384.0f);

  //We use a certain proportion because the creature's body doesn't occupy the entirety of the image
  base_sprite.setScale(creature->GetSize()/208.0f, creature->GetSize()/208.0f);
  eyes_sprite.setScale(creature->GetSize()/208.0f, creature->GetSize()/208.0f);
  tail_sprite.setScale(creature->GetSize()/208.0f, creature->GetSize()/208.0f);

  // Rotation offset
  base_sprite.setRotation(90.0f);
  eyes_sprite.setRotation(90.0f);
  tail_sprite.setRotation(90.0f);

  //Add color filter to creature
  texture_manager_.color_shader_.setUniform("hueShift", creature->GetColor());

  sf::Transform creatureTransform;
  creatureTransform.translate(position.first,
                              position.second);
  creatureTransform.rotate(creature->GetOrientation() * 180.0f /M_PI);

  sf::RenderStates states;
  states.shader = &texture_manager_.color_shader_;
  states.transform = creatureTransform;

  draw(base_sprite, states);
  draw(eyes_sprite, states);
  draw(tail_sprite, states);
}

void SimulationCanvas::RenderPheromoneAtPosition(const std::shared_ptr<Pheromone> pheromone, const std::pair<double, double>& position){
  sf::Sprite pheromoneSprite;
  pheromoneSprite.setTexture(texture_manager_.pheromone_texture_);
  pheromoneSprite.setOrigin(128.0f, 128.0f);

  pheromoneSprite.setScale(pheromone->GetSize()/128.0f, pheromone->GetSize()/128.0f);

  texture_manager_.color_shader_.setUniform("hueShift", pheromone->GetColor());

  sf::Transform pheromoneTransform;
  pheromoneTransform.translate(position.first, position.second);

  sf::RenderStates states;
  states.shader = &texture_manager_.color_shader_;
  states.transform = pheromoneTransform;

  draw(pheromoneSprite, states);
}

std::vector<std::pair<double, double>> SimulationCanvas::getEntityRenderPositions(const std::shared_ptr<Entity> entity) {
    std::vector<std::pair<double, double>> positions;
    if (entity == nullptr) return positions;
    auto [entityX, entityY] = entity->GetCoordinates();
    double entitySize = entity->GetSize();
    sf::Vector2f viewCenter = ui_view_.getCenter();
    sf::Vector2f viewSize = ui_view_.getSize();
    double mapWidth = SETTINGS.environment.map_width;
    double mapHeight = SETTINGS.environment.map_height;

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
      {entityX + mapWidth, entityY},
      {entityX - mapWidth, entityY},
      {entityX, entityY + mapHeight},
      {entityX, entityY - mapHeight},
      {entityX + mapWidth, entityY + mapHeight},
      {entityX - mapWidth, entityY - mapHeight},
      {entityX + mapWidth, entityY - mapHeight},
      {entityX - mapWidth, entityY + mapHeight}};

  for (const auto& pos : potentialPositions) {
    if (isInView(pos.first, pos.second)) {
      positions.push_back(pos);
    }
  }

  return positions;
}


bool SimulationCanvas::event(QEvent* event) {
  if (event->type() == QEvent::Gesture)
    return gestureEvent(static_cast<QGestureEvent*>(event));
  return QWidget::event(event);
}

void SimulationCanvas::centerViewAroundCreature(const sf::Vector2f& creaturePosition) {
    // Calculate the new center directly based on the creature position
    sf::Vector2f newCenter = creaturePosition;
    // Set the new center of the view
    ui_view_.setCenter(newCenter);
    // Set the new size of the view (you may adjust this based on your requirements)
    ui_view_.setSize(ui_view_.getSize().x, ui_view_.getSize().y);
    // Apply the new view
    setView(ui_view_);
    currTopLeft = ui_view_.getCenter() - sf::Vector2f(ui_view_.getSize().x / 2, ui_view_.getSize().x / 2);
}

void SimulationCanvas::mousePressEvent(QMouseEvent* event) {
  float scaleFactor = this->devicePixelRatioF();
  sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                         static_cast<int>(event->position().y() * scaleFactor));
  sf::Vector2f mousePos = mapPixelToCoords(scaledPos);

  qDebug() << "Top Left: " << currTopLeft.x << "  " << currTopLeft.y;
  float screenHeight = sf::VideoMode::getDesktopMode().height;
  float scaledScreenHeight = static_cast<float>(screenHeight / scaleFactor);
  float screenWidth = sf::VideoMode::getDesktopMode().width;
  float scaledScreenWidth = static_cast<float>(screenWidth / scaleFactor);
  float mouseWindowHeight = sf::Mouse::getPosition(*this).y;
  float mouseWindowWidth = sf::Mouse::getPosition(*this).x;

  qDebug() << "Following?: " << followCreature;
  //qDebug() << "This one: " << followedCreature;

  qDebug() << "Height of box: " << scaledScreenHeight;

  // Needed for differentiating clicking and dragging
  initialClickPosition = mousePos;

  sf::Vector2f warpedMousePos;
  warpedMousePos.x = fmod(mousePos.x + static_cast<float>(SETTINGS.environment.map_width), static_cast<float>(SETTINGS.environment.map_width));
  warpedMousePos.y = fmod(mousePos.y + static_cast<float>(SETTINGS.environment.map_height), static_cast<float>(SETTINGS.environment.map_height));

  isClicking = true;

  qDebug() << "Mouse Pressed at: " << mousePos.x << ", " << mousePos.y;

  auto data = simulation_->GetSimulationData();

  float scaledX = initialClickPosition.x;
  float scaledY = initialClickPosition.y;
  if (QSysInfo::kernelType() == "darwin") {
          qDebug() << "True";
          scaledX=currTopLeft.x+ui_view_.getSize().x*(mouseWindowWidth/scaledScreenWidth);
          scaledY=currTopLeft.y+ui_view_.getSize().y*(mouseWindowHeight/scaledScreenHeight);
  }

  if (event->button() == Qt::RightButton) {
          rightClickStartPosition_ = sf::Mouse::getPosition(*this);
          rightClickCreature_ = nullptr;
          // Right-clicked, check if the mouse position coincides with any creature
          bool foundCreature = false;
          for (auto& creature : simulation_->GetSimulationData()->creatures_) {
              auto [creatureX, creatureY] = creature->GetCoordinates();
              float creatureSize = creature->GetSize();
              sf::Vector2f creaturePos(creatureX, creatureY);

              if (sqrt(pow(scaledX - creaturePos.x, 2) + pow(scaledY - creaturePos.y, 2)) <= 1.5 * creatureSize) {
                  // If not following any creature or following a different creature,
                  // update the following creature immediately
                  qDebug() << "First case";
                  if (!followCreature ) {
                      foundCreature = true;
                      followCreature = true;
                      followedCreature = creature;  // Pass the shared pointer directly
                      centerViewAroundCreature(creaturePos);

                      // Update the view
                      info_panel_.Show();
                      info_panel_.SetSelectedCreature(creature);
                      repaint();
                      update();
                      float viewWidth = ui_view_.getSize().x;
                      float viewHeight = ui_view_.getSize().y;
                      currTopLeft = ui_view_.getCenter() - sf::Vector2f(viewWidth / 2, viewHeight / 2);
                  }
                  else if ((followedCreature->GetCoordinates() != creature->GetCoordinates())){
                    qDebug() << "HAPPENING";
                    followedCreature = creature;
                    centerViewAroundCreature(creaturePos);
                    info_panel_.Show();
                    info_panel_.SetSelectedCreature(creature);
                    repaint();
                    update();
                  }
                  break;  // Exit the loop after finding the first creature at the clicked position
              }
          }

          // If no creature was found at the clicked position, reset followCreature
          if (!foundCreature && followCreature) {
              followCreature = false;
              followedCreature = nullptr;
              update();
              currTopLeft = ui_view_.getCenter() - sf::Vector2f(ui_view_.getSize().x / 2, ui_view_.getSize().x / 2);
          }

          return;
      } else {
          // Any other mouse click while following a creature stops following
          if (followCreature) {
              followCreature = false;
              followedCreature = nullptr;
              update();
              currTopLeft = ui_view_.getCenter() - sf::Vector2f(ui_view_.getSize().x / 2, ui_view_.getSize().x / 2);
          }
      }

  // Checking if we need to display info panel
  for (auto& creature : data->creatures_) {
    auto [creatureX, creatureY] = creature->GetCoordinates();
    float creatureSize = creature->GetSize();
    sf::Vector2f creaturePos(creatureX, creatureY);

    if (sqrt(pow(warpedMousePos.x - creaturePos.x, 2) + pow(warpedMousePos.y - creaturePos.y, 2)) <= creatureSize) {
      qDebug() << "Creature Clicked: ID" << creature->GetID();
      info_panel_.Show();
      info_panel_.SetSelectedCreature(creature);
      info_panel_.SetOffset(mousePos.x - warpedMousePos.x, mousePos.y - warpedMousePos.y);
      repaint();
      return;
    }
  }

  qDebug() << "Click Outside, closing panel";
  info_panel_.Hide();
  info_panel_.SetSelectedCreature(nullptr);
  // repaint();
}


void SimulationCanvas::wheelEvent(QWheelEvent* event) {
  qDebug() << "Wheel event";
  // Use platform-independent pixelDelta for wheel events
  QPoint pixelDelta = event->pixelDelta();
  QPoint angleDelta = event->angleDelta();

  if (!pixelDelta.isNull()) {
    // Use pixelDelta for more accurate scrolling
    float delta = pixelDelta.y() > 0 ? 1.1f : 0.9f;
    zoom(1/delta);
  } else if (!angleDelta.isNull()) {
    // Use angleDelta as a fallback
    float delta = angleDelta.y() > 0 ? 1.1f : 0.9f;
    zoom(1/delta);
  }
}

void SimulationCanvas::zoom(float factor) {;
  // Check for minimum and maximum zoom
  if (zoomFactor * factor < SETTINGS.ui.min_zoom) {
    factor = SETTINGS.ui.min_zoom / zoomFactor;
  } else if (zoomFactor * factor > SETTINGS.ui.max_zoom) {
    factor = SETTINGS.ui.max_zoom / zoomFactor;
  }

  zoomFactor *= factor;
  qDebug() << "New zoom factor" << zoomFactor;

  // Get the current mouse position in screen coordinates
  sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(*this);

  // Convert screen coordinates to world coordinates
  sf::Vector2f mouseWorldPosBeforeZoom = mapPixelToCoords(mouseScreenPos);

  // Calculate the new center directly based on the mouse position
  sf::Vector2f newCenter = mouseWorldPosBeforeZoom;

  // Adjust the new center based on the zoom factor and direction
  float zoomSpeed = abs(1 - factor);
  sf::Vector2f centerDiff = newCenter - ui_view_.getCenter();
  if (factor > 1.0f) {
    // Invert the direction for zooming out
    centerDiff = -centerDiff;
  }
  ui_view_.setCenter(ui_view_.getCenter() + centerDiff * zoomSpeed);

  // After adjusting center, get the new mouse position in world coordinates
  sf::Vector2f mouseWorldPosAfterZoom = mapPixelToCoords(mouseScreenPos);
  // Adjust the center again to ensure it's relative to the mouse position
  sf::Vector2f zoomDiff = mouseWorldPosBeforeZoom - mouseWorldPosAfterZoom;
  ui_view_.setCenter(ui_view_.getCenter() + zoomDiff * factor);

  // Set the new size of the view
  ui_view_.setSize(initialViewSize.x * zoomFactor,
                   initialViewSize.y * zoomFactor);

  // Apply the new view and update
  setView(ui_view_);
  currTopLeft = ui_view_.getCenter() - sf::Vector2f(ui_view_.getSize().x / 2, ui_view_.getSize().y / 2);
  update();
}

void SimulationCanvas::mouseMoveEvent(QMouseEvent* event) {
  float scaleFactor = this->devicePixelRatioF();
  sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                         static_cast<int>(event->position().y() * scaleFactor));
  sf::Vector2f currentMousePosition = mapPixelToCoords(scaledPos);

  if (isClicking) {
    float distance =
        sqrt(pow(currentMousePosition.x - initialClickPosition.x, 2) +
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

    sf::Vector2f averagedDelta = std::accumulate(
        deltaHistory.begin(), deltaHistory.end(), sf::Vector2f(0, 0));
    averagedDelta /= static_cast<float>(deltaHistory.size());

        ui_view_.move(averagedDelta);
        // Get map dimensions
        double mapWidth = SETTINGS.environment.map_width;
        double mapHeight = SETTINGS.environment.map_height;

    // Get the new center of the view
    sf::Vector2f newCenter = ui_view_.getCenter();

    // Wrap around horizontally
    if (newCenter.x < 0) {
      currentMousePosition.x += mapWidth;
      newCenter.x += mapWidth;
    } else if (newCenter.x > mapWidth) {
      currentMousePosition.x -= mapWidth;
      newCenter.x -= mapWidth;
    }

    // Wrap around vertically
    if (newCenter.y < 0) {
      currentMousePosition.y += mapHeight;
      newCenter.y += mapHeight;
    } else if (newCenter.y > mapHeight) {
      currentMousePosition.y -= mapHeight;
      newCenter.y -= mapHeight;
    }
    // Set the wrapped center back to the view
    ui_view_.setCenter(newCenter);
    setView(ui_view_);

    initialClickPosition = currentMousePosition;

    //sf::Vector2f currentCenter = ui_view_.getCenter();

  }
  float viewWidth = ui_view_.getSize().x;
  float viewHeight = ui_view_.getSize().y;
  currTopLeft = ui_view_.getCenter() - sf::Vector2f(viewWidth / 2, viewHeight / 2);
}

void SimulationCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
            sf::Vector2i rightClickEndPosition = sf::Mouse::getPosition(*this);
            sf::Vector2f worldRightClickStart = mapPixelToCoords(rightClickStartPosition_);
            sf::Vector2f worldRightClickEnd = mapPixelToCoords(rightClickEndPosition);

            // Find the creature clicked during right-click
            for (auto& creature : simulation_->GetSimulationData()->creatures_) {
                auto [creatureX, creatureY] = creature->GetCoordinates();
                float creatureSize = creature->GetSize();
                sf::Vector2f creaturePos(creatureX, creatureY);

                if (pow(worldRightClickEnd.x - creaturePos.x, 2) + pow(worldRightClickEnd.y - creaturePos.y, 2) <= pow(1.5 * creatureSize, 2)) {
                    rightClickCreature_ = creature;
                    break;
                }
            }
        }
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
  info_panel_view_ = view;
}

InfoPanel& SimulationCanvas::GetInfoPanel() { return info_panel_; }
