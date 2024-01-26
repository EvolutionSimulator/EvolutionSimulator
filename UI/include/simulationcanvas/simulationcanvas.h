#pragma once

#include <engine.h>

#include <SFML/Graphics.hpp>
#include <QGestureEvent>

#include "qwidgets/qsfmlcanvas.h"
#include "simulation.h"
#include "info_panel.h"
#include "texture_manager.h"

class SimulationCanvas : public QSFMLCanvas {
 Q_OBJECT

 public:

  SimulationCanvas(QWidget* Parent);

  void SetSimulation(Simulation* simulation);
  Simulation* GetSimulation();

  void UpdateFoodDensityTexture(SimulationData& data);

  InfoPanel& GetInfoPanel();

  protected:
  bool event(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  void zoom(float factor);
  void centerViewAroundCreature(const sf::Vector2f& creaturePosition);


  private:
  Simulation *simulation_ = nullptr;
  virtual void OnInit() override;

  virtual void OnUpdate() override;

  bool gestureEvent(QGestureEvent* event);
  void pinchTriggered(QPinchGesture* gesture);

  std::shared_ptr<Creature> followedCreature = nullptr;
  bool followCreature = false;
  sf::Vector2f currTopLeft = sf::Vector2f(0,0);
  sf::Vector2i rightClickStartPosition_;
  std::shared_ptr<Creature> rightClickCreature_;

  // Rendering logic
  void RenderSimulation(SimulationData& data);
  void RenderFoodAtPosition(const std::shared_ptr<Food> food, const std::pair<double, double>& position);
  void RenderEggAtPosition(const std::shared_ptr<Egg> egg, const std::pair<double, double>& position);
  void RenderCreatureAtPosition(const std::shared_ptr<Creature> creature, const std::pair<double, double>& position);
  void RenderPheromoneAtPosition(const std::shared_ptr<Pheromone> pheromone, const std::pair<double, double> &position);
  std::vector<std::pair<double, double>> getEntityRenderPositions(const std::shared_ptr<Entity> entity);

  // Zoom logic
  float zoomFactor = 1.0f;
  sf::Vector2f initialViewSize;
  sf::Vector2f initialViewCenter;

  // CLick and drag logic
  bool isDragging = false;
  sf::Vector2f initialClickPosition;
  bool isClicking = false;
  std::deque<sf::Vector2f> deltaHistory;

  // Textures
  TextureManager texture_manager_;

  // Info panel
  InfoPanel info_panel_;
  void SetSelectedFood();
  void DrawMouseCoordinates();

  sf::View ui_view_;
  sf::View info_panel_view_;
};

