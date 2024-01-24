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

void InfoPanel::SetUIView(sf::View view){ ui_view_ = view;}

void InfoPanel::SetPanelView(sf::View view){ info_panel_view_ = view;}


void InfoPanel::SetOffset(float offset_x, float offset_y) {offset_x_ = offset_x; offset_y_ = offset_y; };

void InfoPanel::SetSelectedCreature(std::shared_ptr<Creature> creature) {
  selected_creature_ = creature;
}

void InfoPanel::UpdateSelectedFood() {closest_entity_ = selected_creature_->GetFoodID();}

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
  DrawStomach(target, *selected_creature_);
  DrawPanel(target);

  auto creature_position = selected_creature_->GetCoordinates();
  DrawVisionCone(target, *selected_creature_, creature_position);
  creature_position.first += offset_x_;
  creature_position.second += offset_y_;
  DrawVisionCone(target, *selected_creature_, creature_position);
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
  ss << "Max Energy: " <<round_double(creature.GetMaxEnergy(), 2) << "\n";
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
  return ss.str();
}

void InfoPanel::DrawPanel(sf::RenderTarget& target) {
  target.setView(info_panel_view_);

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

  // Prepare and draw the creature info text inside the panel
  sf::Text infoText;
  infoText.setFont(texture_manager_->font_);
  infoText.setString(creature_info.toStdString());
  infoText.setCharacterSize(15);
  infoText.setFillColor(sf::Color::White);
  infoText.setPosition(panelPosition.x + 10, 10);  // Adjust the Y position as needed
  target.draw(infoText);

  target.setView(ui_view_);

  sf::CircleShape redCircle((*selected_creature_).GetSize()); // Adjust as needed
  redCircle.setOutlineColor(sf::Color::Red);
  redCircle.setOutlineThickness((*selected_creature_).GetSize()/5); // Adjust thickness as needed
  redCircle.setFillColor(sf::Color::Transparent);
  redCircle.setPosition((*selected_creature_).GetCoordinates().first - (*selected_creature_).GetSize() + offset_x_,
                        (*selected_creature_).GetCoordinates().second - (*selected_creature_).GetSize() + offset_y_);
  target.draw(redCircle);
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

  if(closest_entity_){
    sf::CircleShape blueCircle(closest_entity_->GetSize()); // Adjust as needed
    blueCircle.setOutlineColor(sf::Color::Blue);
    blueCircle.setOutlineThickness(2); // Adjust thickness as needed
    blueCircle.setFillColor(sf::Color::Transparent);

    blueCircle.setPosition(closest_entity_->GetCoordinates().first - closest_entity_->GetSize(),
                           closest_entity_->GetCoordinates().second - closest_entity_->GetSize());
    target.draw(blueCircle);
    blueCircle.setPosition(closest_entity_->GetCoordinates().first - closest_entity_->GetSize() + offset_x_,
                           closest_entity_->GetCoordinates().second - closest_entity_->GetSize() + offset_y_);
    target.draw(blueCircle);
    blueCircle.setPosition(selected_food_->GetCoordinates().first - selected_food_->GetSize(),
                           selected_food_->GetCoordinates().second - selected_food_->GetSize());
    target.draw(blueCircle);
  }


}

void InfoPanel::DrawVisionCone(sf::RenderTarget& target, const Creature &creature, std::pair<double, double> position) {

  target.setView(ui_view_);

  double visionRadius = creature.GetVisionRadius();
  double visionAngle = creature.GetVisionAngle();

  double creatureOrientation = creature.GetOrientation();

  double leftRad = creatureOrientation - visionAngle / 2.0;
  double rightRad = creatureOrientation + visionAngle / 2.0;

  std::vector<sf::Vertex> triangleFan;
  triangleFan.push_back(sf::Vertex(sf::Vector2f(position.first, position.second), sf::Color::Transparent));

  int numPoints = 30;
  for (int i = 0; i <= numPoints; ++i) {
    double angle = leftRad + (i * (rightRad - leftRad) / numPoints);
    double x = position.first + visionRadius * cos(angle);
    double y = position.second + visionRadius * sin(angle);
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

void InfoPanel::DrawStomach(sf::RenderTarget& target, const Creature& creature) {

  target.setView(info_panel_view_);
  // Use the window size to determine the position of the stomach images
  sf::Vector2u windowSize = target.getSize();
  float margin = 10.0f;  // Margin from the bottom left corner

         // Fullness sprite setup
  sf::Sprite stomachFullnessSprite;
  stomachFullnessSprite.setTexture(texture_manager_->stomach_texture_);
  stomachFullnessSprite.setOrigin(0, 0);
  float consistentScaleFactor = 1.0f; // Scaling factor for the sprite
  stomachFullnessSprite.setScale(consistentScaleFactor, consistentScaleFactor);

         // Position for the fullness stomach sprite at the bottom left corner
  float posX = margin; // 10 pixels from the left
  float posY = windowSize.y - texture_manager_->stomach_texture_.getSize().y * consistentScaleFactor - 2 * margin; // 20 pixels from the bottom
  stomachFullnessSprite.setPosition(posX, posY);

         // Acid sprite setup (positioned above the fullness sprite)
  sf::Sprite stomachAcidSprite;
  stomachAcidSprite.setTexture(texture_manager_->stomach_texture_);
  stomachAcidSprite.setOrigin(0, 0);
  stomachAcidSprite.setScale(consistentScaleFactor, consistentScaleFactor);

         // Position for the acid stomach sprite above the fullness sprite
         // The additional margin is for spacing between the two sprites
  float additionalMargin = 130.0f; // Adjust this value as needed
  float posYAcid = posY - additionalMargin;
  stomachAcidSprite.setPosition(posX, posYAcid);

         // Set uniform variables for the fullness shader
  float fullnessRatio = creature.GetStomachFullness() / creature.GetStomachCapacity();
  texture_manager_->stomach_shader_.setUniform("fullness", fullnessRatio);
  texture_manager_->stomach_shader_.setUniform("texture", sf::Shader::CurrentTexture);
  texture_manager_->stomach_shader_.setUniform("fillColor", sf::Glsl::Vec4(sf::Color(255, 0, 255))); // Magenta for fullness

         // Draw the fullness sprite with the shader
  target.draw(stomachFullnessSprite, &texture_manager_->stomach_shader_);

  sf::Text foodLabel;
  foodLabel.setFont(texture_manager_->font_);
  foodLabel.setString("Food");
  foodLabel.setCharacterSize(24); // Or another appropriate size
  foodLabel.setFillColor(sf::Color::White); // Or another color
  // Set position for the food label above the fullness sprite
  foodLabel.setPosition(posX + 52.0f, posY - foodLabel.getLocalBounds().height + 20.0f); // Adjust Y position based on height of text and desired ma

  sf::Text acidLabel;
  acidLabel.setFont(texture_manager_->font_);
  acidLabel.setString("Acid");
  acidLabel.setCharacterSize(24); // Or another appropriate size
  acidLabel.setFillColor(sf::Color::White); // Or another color
  // Set position for the acid label above the acid sprite
  acidLabel.setPosition(posX + 57.0f , posYAcid - acidLabel.getLocalBounds().height + 20.0f); // Adjust Y position based on height of text and desired margin


         // Set uniform variables for the acid shader
  float acidRatio = creature.GetAcid() / creature.GetStomachCapacity();
  texture_manager_->stomach_shader_.setUniform("fullness", acidRatio);
  texture_manager_->stomach_shader_.setUniform("texture", sf::Shader::CurrentTexture);
  texture_manager_->stomach_shader_.setUniform("fillColor", sf::Glsl::Vec4(sf::Color(255, 255, 0))); // Yellow for acid

         // Draw the acid sprite with the shader
  target.draw(stomachAcidSprite, &texture_manager_->stomach_shader_);

  target.draw(foodLabel);
  target.draw(acidLabel);
}
