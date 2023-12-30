#pragma once

#include <core/engine.h>

#include <SFML/Graphics.hpp>

#include "qsfmlcanvas.h"
#include "simulation/simulation.h"

class SimulationCanvas : public QSFMLCanvas {
 Q_OBJECT

 public:
 sf::RectangleShape trackButton_;
 sf::Text trackButtonText_;
 bool trackButtonVisible_ = false;
 struct CreatureInfo {
     int id = -1; // ID of the creature, initialize with an invalid value
     double x = 0.0;
     double y = 0.0;
     float size = 0.0f;
     // You might want to add other properties like health, age, etc., if necessary
 };

  sf::RenderWindow& GetRenderWindow();

  SimulationCanvas(QWidget* Parent);

  void SetSimulation(Simulation* simulation);
  Simulation* GetSimulation();

  void DrawGraph(sf::RenderWindow& window, double (*graphFunction)(double),
                 double xMin, double xMax, double yMin, double yMax);

  // New function declaration for drawing the number of creatures over time
  void DrawCreatureCountOverTime(sf::RenderWindow &window, const std::vector<Creature> &creatures);
  void DrawVisionCone(sf::RenderTarget& target, const Creature& creature);

  protected:
  void mousePressEvent(QMouseEvent *event) override;

  private:
  virtual void OnInit() override;
  virtual void OnUpdate() override;
  std::optional<CreatureInfo> selectedCreatureInfo;


  sf::RenderWindow renderWindow_;

  sf::Font font_;

  bool showInfoPanel;
  std::optional<std::pair<float, float>> clickedCreaturePos;
  QString creatureInfo;
  QRectF panelRect;

  void RenderSimulation(DataAccessor<SimulationData> data);

  Simulation* simulation_ = nullptr;

  bool isCreatureClicked(const sf::Vector2f &mousePos);

  void displayInfoPanel();
  std::string formatCreatureInfo(const Creature &creature);
};
