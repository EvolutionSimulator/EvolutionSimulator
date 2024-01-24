#include "simulationcanvas/simulationcanvas.h"
#include "QtWidgets/qapplication.h"
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
      info_panel_(InfoPanel(this, &texture_manager_, simulation_))
{

 // Scale the map width and height according to the pixel ratio
  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio

  int scaledWidth = static_cast<int>(width/scaleFactor);
  int scaledHeight = static_cast<int>(height/scaleFactor);

  // Set the fixed size with the scaled dimensions
  setFixedSize(scaledWidth, scaledHeight);

  std::cout << "SimulationCanvas created" << std::endl;
}




void SimulationCanvas::UpdateFoodDensityTexture(double width, double height){
    texture_manager_.food_density_texture_.create(width, height);
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
    texture_manager_.food_density_texture_.setSmooth(true);
    // Load the image into the texture
    texture_manager_.food_density_texture_.update(densityImage);
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
    if (followCreature && followedCreature) {
        // Update the view to follow the creature
        centerViewAroundCreature({static_cast<float>(followedCreature->GetCoordinates().first), static_cast<float>(followedCreature->GetCoordinates().second)});
    }

  RenderSimulation(simulation_->GetSimulationData());

  // Check if a creature is selected
  if (info_panel_.IsVisible() && info_panel_.GetSelectedCreature()) {
        info_panel_.UpdateSelectedFood();
        info_panel_.Draw();
  }else if (info_panel_.IsVisible()) {
    std::cout << "Info panel flag is set, but no creature position is recorded."
              << std::endl;
  }
  DrawMouseCoordinates();
}

void SimulationCanvas::DrawMouseCoordinates()
{
  sf::Vector2i mousePixelPos = sf::Mouse::getPosition(*this);
  sf::Vector2f mousePos = mapPixelToCoords(mousePixelPos);

  // Prepare the text to display the mouse coordinates
  sf::Text mouseCoordsText;
  mouseCoordsText.setFont(texture_manager_.font_);
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

// use this to process the simulation data and render it on the screen
void SimulationCanvas::RenderSimulation(DataAccessor<SimulationData> data) {
  clear(sf::Color(20, 22, 69));

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
  foodSprite.setTexture(texture_manager_.food_texture_);
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
  base_sprite.setTexture(texture_manager_.creature_texture_);
  eyes_sprite.setTexture(texture_manager_.eyes_texture_);
  tail_sprite.setTexture(texture_manager_.tail_texture_);

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

  //Add color filter to creature
  texture_manager_.color_shader_.setUniform("hueShift", creature.GetMutable().GetColor());

  sf::Transform creatureTransform;
  creatureTransform.translate(position.first,
                              position.second);
  creatureTransform.rotate(creature.GetOrientation() * 180.0f /M_PI);

  sf::RenderStates states;
  states.shader = &texture_manager_.color_shader_;
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


void SimulationCanvas::centerViewAroundCreature(const sf::Vector2f& creaturePosition) {
    sf::View view = getView();
    // Calculate the new center directly based on the creature position
    sf::Vector2f newCenter = creaturePosition;
    // Set the new center of the view
    view.setCenter(newCenter);
    // Set the new size of the view (you may adjust this based on your requirements)
    view.setSize(getView().getSize().x, getView().getSize().y);
    // Apply the new view
    setView(view);
}

void SimulationCanvas::mousePressEvent(QMouseEvent* event) {
    float scaleFactor = this->devicePixelRatioF();
    qDebug() << "Top Left: " << currTopLeft.x << "  " << currTopLeft.y;
    float screenHeight = sf::VideoMode::getDesktopMode().height;
    float scaledScreenHeight = static_cast<float>(screenHeight / scaleFactor);
    float screenWidth = sf::VideoMode::getDesktopMode().width;
    float scaledScreenWidth = static_cast<float>(screenWidth / scaleFactor);
    float mouseWindowHeight = sf::Mouse::getPosition(*this).y;
    float mouseWindowWidth = sf::Mouse::getPosition(*this).x;

    qDebug() << "Following?: " << followCreature;
    qDebug() << "This one: " << followedCreature;

    qDebug() << "Height of box: " << scaledScreenHeight;
    sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                           static_cast<int>(event->position().y() * scaleFactor));
    sf::Vector2f mousePos = mapPixelToCoords(scaledPos);

    // Needed for differentiating clicking and dragging
    initialClickPosition = mousePos;
    isClicking = true;
    //qDebug() << "Mouse Pressed at: " << mousePos.x << ", " << mousePos.y;

    auto data = simulation_->GetSimulationData();

    float scaledX = initialClickPosition.x;
//    //scaledX = fmod(scaledX, static_cast<float>(settings::environment::kDMapWidth));
    float scaledY = initialClickPosition.y;
//    //scaledY = fmod(scaledY, static_cast<float>(settings::environment::kDMapHeight));
//    scaledX/=scaleFactor; //1439/2880
//    scaledY/=scaleFactor; //899/1800
//    qDebug() << "(RESCALED) Mouse Pressed at: " << scaledX << ", " << scaledY;
    if (QSysInfo::kernelType() == "darwin") {
            qDebug() << "True";
            scaledX=currTopLeft.x+getView().getSize().x*(mouseWindowWidth/scaledScreenWidth);
            scaledY=currTopLeft.y+getView().getSize().y*(mouseWindowHeight/scaledScreenHeight);
    }


    if (event->button() == Qt::RightButton) {
        // Right-clicked, check if the mouse position coincides with any creature
        bool foundCreature = false;

        for (auto& creature : simulation_->GetSimulationData()->creatures_) {
            auto [creatureX, creatureY] = creature.GetCoordinates();
            float creatureSize = creature.GetSize();
            sf::Vector2f creaturePos(creatureX, creatureY);

            if (sqrt(pow(scaledX - creaturePos.x, 2) + pow(scaledY - creaturePos.y, 2)) <= 1.5 * creatureSize) {
                if (!followCreature || (followCreature && followedCreature != &creature)) {
                    // If not following any creature or following a different creature,
                    // update the following creature immediately
                    foundCreature = true;
                    followCreature = true;
                    followedCreature = &creature;
                    centerViewAroundCreature(creaturePos);

                    // Update the view
                    info_panel_.Show();
                    info_panel_.SetSelectedCreature(&creature);
                    repaint();
                    update();
                    currTopLeft = getView().getCenter() - sf::Vector2f(getView().getSize().x / 2, getView().getSize().x / 2);
                }
                break;  // Exit the loop after finding the first creature at the clicked position
            }
        }

        // If no creature was found at the clicked position, reset followCreature
        if (!foundCreature && followCreature) {
            followCreature = false;
            followedCreature = nullptr;
            update();
            currTopLeft = getView().getCenter() - sf::Vector2f(getView().getSize().x / 2, getView().getSize().x / 2);
        }

        return;
    } else {
        // Any other mouse click while following a creature stops following
        if (followCreature) {
            followCreature = false;
            followedCreature = nullptr;
            update();
            currTopLeft = getView().getCenter() - sf::Vector2f(getView().getSize().x / 2, getView().getSize().x / 2);
        }
    }


    // Checking if we need to display info panel
    for (auto& creature : data->creatures_) {
        auto [creatureX, creatureY] = creature.GetCoordinates();
        float creatureSize = creature.GetSize();
        sf::Vector2f creaturePos(creatureX, creatureY);

        if (abs(creatureY-scaledY)<20 && (abs(creatureX - scaledX) < 20)){
            qDebug() << "Creature Position: " << creatureX << "    " << creatureY;
        }

        if (sqrt(pow(scaledX - creaturePos.x, 2) + pow(scaledY - creaturePos.y, 2)) <= 1.5*creatureSize) {
            qDebug() << "Creature Clicked: ID" << creature.GetID();
            info_panel_.Show();
            info_panel_.SetSelectedCreature(&creature);
            repaint();
            return;
        }
    }

    qDebug() << "Click Outside, closing panel";
    info_panel_.Hide();
    info_panel_.SetSelectedCreature(nullptr);
    repaint();
}



void SimulationCanvas::wheelEvent(QWheelEvent *event) {

    // Use platform-independent pixelDelta for wheel events
    QPoint pixelDelta = event->pixelDelta();
    QPoint angleDelta = event->angleDelta();
    float scaleFactor = this->devicePixelRatioF();
    sf::Vector2i scaledPos(static_cast<int>(event->position().x() * scaleFactor),
                               static_cast<int>(event->position().y() * scaleFactor));

    if (!pixelDelta.isNull()) {
        // Use pixelDelta for more accurate scrolling
        float delta = pixelDelta.y() > 0 ? 1.1f : 0.9f;
        sf::Vector2f worldPos = mapPixelToCoords(scaledPos);
        zoom(delta, worldPos);
    } else if (!angleDelta.isNull()) {
        // Use angleDelta as a fallback
        float delta = angleDelta.y() > 0 ? 1.1f : 0.9f;
        sf::Vector2f worldPos = mapPixelToCoords(scaledPos);
        zoom(delta, worldPos);
    }
}

void SimulationCanvas::zoom(float factor, sf::Vector2f& zoomPoint) {
    // Get the current view
    sf::View view = getView();

    // Check for maximum zoom
    if (zoomFactor * factor > 1) {
        zoomFactor = 1;
        view.setCenter(initialViewCenter);
        currTopLeft=sf::Vector2f(0,0);
    } else {
        zoomFactor *= factor;

        // Get the current mouse position in screen coordinates
        sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(*this);

        // Convert screen coordinates to world coordinates
        sf::Vector2f mouseWorldPosBeforeZoom = mapPixelToCoords(mouseScreenPos);

        // Calculate the new center directly based on the mouse position
        sf::Vector2f newCenter = mouseWorldPosBeforeZoom;

        // Adjust the new center based on the zoom factor and direction
        float zoomSpeed = 0.1f;
        sf::Vector2f centerDiff = newCenter - view.getCenter();
        if (factor > 1.0f) {
            // Invert the direction for zooming out
            centerDiff *= -(1.0f + 1.0f / (8*factor));
        }
        view.setCenter(view.getCenter() + centerDiff * zoomSpeed);

        // After adjusting center, get the new mouse position in world coordinates
        sf::Vector2f mouseWorldPosAfterZoom = mapPixelToCoords(mouseScreenPos);
        // Adjust the center again to ensure it's relative to the mouse position
        sf::Vector2f zoomDiff = mouseWorldPosBeforeZoom - mouseWorldPosAfterZoom;
        view.setCenter(view.getCenter() + zoomDiff * factor);

        // Update currTopLeft based on the new view center

    }

    // Set the new size of the view
    view.setSize(initialViewSize.x * zoomFactor, initialViewSize.y * zoomFactor);

    // Apply the new view and update
    setView(view);
    currTopLeft = view.getCenter() - sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2);
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
        sf::Vector2f currentCenter = view.getCenter();
        float viewWidth = view.getSize().x;
        float viewHeight = view.getSize().y;

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
//        sf::Vector2f currentCenter = view.getCenter();
//        float viewWidth = view.getSize().x;
//        float viewHeight = view.getSize().y;

        // Adjust the averaged delta based on the view size
        averagedDelta.x *= viewWidth / getSize().x;
        averagedDelta.y *= viewHeight / getSize().y;

        // Update the view position
        view.setCenter(currentCenter + averagedDelta);
        setView(view);

        initialClickPosition = currentMousePosition;

        // Update currTopLeft based on the new view center
        currTopLeft = view.getCenter() - sf::Vector2f(viewWidth / 2, viewHeight / 2);
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

InfoPanel& SimulationCanvas::GetInfoPanel() {
    return info_panel_;
}
