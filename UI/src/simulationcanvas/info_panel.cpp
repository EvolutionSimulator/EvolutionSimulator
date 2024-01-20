#include "simulationcanvas/info_panel.h"
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

InfoPanel::InfoPanel(QSFMLCanvas* canvas, TextureManager *texture_manager, Simulation* simulation)
    : canvas_(canvas),
      selected_creature_(nullptr),
      is_visible_(false),
      texture_manager_(texture_manager),
      simulation_(simulation)
{

}

void InfoPanel::SetSelectedCreature(std::shared_ptr<Creature> creature) {
  selected_creature_ = creature;
}

void InfoPanel::UpdateSelectedFood() {
  selected_food_ = selected_creature_->GetFoodID();
}

std::shared_ptr<Creature> InfoPanel::GetSelectedCreature() const {
  return selected_creature_;
}

void InfoPanel::Show() {
  is_visible_ = true;
}

void InfoPanel::Hide() {
  is_visible_ = false;
}

bool InfoPanel::IsVisible() const {
  return is_visible_;
}

void InfoPanel::Draw() {
  if (!is_visible_ || !selected_creature_ || !canvas_) return;
  sf::RenderTarget& target = *canvas_;
  DrawPanel(target);
  DrawVisionCone(target, *selected_creature_);
}

double round_double(double number, int decimal_places) {
  const double multiplier = std::pow(10.0, decimal_places);
  return std::round(number * multiplier) / multiplier;
}

std::string InfoPanel::FormatCreatureInfo(const Creature& creature) {
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

void InfoPanel::DrawPanel(sf::RenderTarget& target) {
  // Right info panel setup
  sf::Vector2f panelSize(200, target.getSize().y);  // Width of 200 and full height of the canvas
  sf::Vector2f panelPosition(target.getSize().x - panelSize.x, 0);  // Positioned on the right side

         // Info panel background
  sf::RectangleShape panel(panelSize);
  panel.setFillColor(sf::Color(50, 50, 20, 205));  // Semi-transparent
  panel.setPosition(panelPosition);
  target.draw(panel);

  auto creature_info = QString::fromStdString(FormatCreatureInfo(*selected_creature_));

  // Draw the energy bar
  float maxBarWidth = 80.0f;  // Width of the full energy bar
  float barHeight = 10.0f;    // Height of the energy bar

  float energyRatio = static_cast<float>((*selected_creature_).GetEnergy()) / static_cast<float>((*selected_creature_).GetMaxEnergy());
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

  sf::Sprite energySprite;
  energySprite.setTexture(texture_manager_->energy_texture_);
  energySprite.setScale(0.04f,0.04f);
  energySprite.setPosition(panelPosition.x + 10, 117);

  target.draw(energySprite);
  target.draw(energyBarOutline);
  target.draw(energyBar);

  float healthRatio = static_cast<float>((*selected_creature_).GetHealth()) / static_cast<float>((*selected_creature_).GetMutable().GetIntegrity() * pow((*selected_creature_).GetSize(), 2));
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

  sf::Sprite healthSprite;
  healthSprite.setTexture(texture_manager_->health_texture_);
  healthSprite.setScale(0.7f,0.7f);
  healthSprite.setPosition(panelPosition.x + 10, 100);

  target.draw(healthSprite);
  target.draw(healthBarOutline);
  target.draw(healthBar);

  sf::CircleShape redCircle((*selected_creature_).GetSize()); // Adjust as needed
  redCircle.setOutlineColor(sf::Color::Red);
  redCircle.setOutlineThickness((*selected_creature_).GetSize()/5); // Adjust thickness as needed
  redCircle.setFillColor(sf::Color::Transparent);
  redCircle.setPosition((*selected_creature_).GetCoordinates().first - (*selected_creature_).GetSize(),
                        (*selected_creature_).GetCoordinates().second - (*selected_creature_).GetSize());
  target.draw(redCircle);

         // Check if the creature's health is 0 and display the message
  if ((*selected_creature_).GetHealth() == 0) {
    creature_info = QString::fromStdString("Creature " + std::to_string((*selected_creature_).GetID()) + " is dead");
  } else {
    // Update the creature info normally
    creature_info = QString::fromStdString(FormatCreatureInfo(*selected_creature_));
  }

  if(selected_food_){
    sf::CircleShape blueCircle(selected_food_->GetSize()); // Adjust as needed
    blueCircle.setOutlineColor(sf::Color::Blue);
    blueCircle.setOutlineThickness(2); // Adjust thickness as needed
    blueCircle.setFillColor(sf::Color::Transparent);
    blueCircle.setPosition(selected_food_->GetCoordinates().first - selected_food_->GetSize(),
                           selected_food_->GetCoordinates().second - selected_food_->GetSize());
    target.draw(blueCircle);
  }

  // Prepare and draw the creature info text inside the panel
  sf::Text infoText;
  infoText.setFont(texture_manager_->font_);
  infoText.setString(creature_info.toStdString());
  infoText.setCharacterSize(15);
  infoText.setFillColor(sf::Color::White);
  infoText.setPosition(panelPosition.x + 10, 10);  // Adjust the Y position as needed
  target.draw(infoText);
}

void InfoPanel::DrawVisionCone(sf::RenderTarget& target, const Creature &creature) {

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
