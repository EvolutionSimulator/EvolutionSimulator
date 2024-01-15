#pragma once

#include <engine.h>

#include <SFML/Graphics.hpp>

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

  void UpdateFoodDensityTexture(double width, double height);

  protected:
  void mousePressEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void zoom(float factor, sf::Vector2f& zoomPoint);

  private:
  Simulation *simulation_ = nullptr;
  virtual void OnInit() override;
  virtual void OnUpdate() override;

  // Rendering logic
  void RenderSimulation(DataAccessor<SimulationData> data);
  void RenderFoodAtPosition(const Food& food, const std::pair<double, double>& position);
  void RenderCreatureAtPosition(const Creature& creature, const std::pair<double, double>& position);
  std::vector<std::pair<double, double>> getEntityRenderPositions(const Entity& entity);

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
};

