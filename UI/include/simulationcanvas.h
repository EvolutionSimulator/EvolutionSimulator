#pragma once

#include <engine.h>

#include <SFML/Graphics.hpp>

#include "qsfmlcanvas.h"
#include "simulation.h"

class SimulationCanvas : public QSFMLCanvas {
 Q_OBJECT

 public:

  SimulationCanvas(QWidget* Parent);

  void SetSimulation(Simulation* simulation);
  Simulation* GetSimulation();

  void DrawVisionCone(sf::RenderTarget& target, const Creature& creature);

  void zoom(float factor, sf::Vector2f& zoomPoint);

  void InitializeFile(sf::Shader& ValueSaved, std::string path);
  void InitializeFile(sf::Font& ValueSaved, std::string path);
  void InitializeFile(sf::Texture& ValueSaved, std::string path);


  void UpdateFoodDensityTexture(double width, double height);

  protected:
  void mousePressEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  private:
  virtual void OnInit() override;
  virtual void OnUpdate() override;

  Creature* selectedCreature;


  sf::Font font_;

  bool showInfoPanel;
  QString creatureInfo;
  QRectF panelRect;

  void RenderSimulation(DataAccessor<SimulationData> data);
  void RenderFoodAtPosition(const Food& food, const std::pair<double, double>& position);
  void RenderCreatureAtPosition(const Creature& creature, const std::pair<double, double>& position);

  Simulation *simulation_ = nullptr;
  std::function<void(SimulationData *)>
      render_lambda_; // we will pass this to the ProcessData method of
                      // Simulation

  bool isCreatureClicked(const sf::Vector2f &mousePos);
  std::vector<std::pair<double, double>> getEntityRenderPositions(const Entity& entity);

  void displayInfoPanel();
  std::string formatCreatureInfo(const Creature &creature);

  float zoomFactor = 1.0f;
  sf::Vector2f initialViewSize;
  sf::Vector2f initialViewCenter;

  sf::Texture texture_;
  double scale_x_;
  double scale_y_;
  sf::Texture food_texture_;
  sf::Texture creature_texture_;
  sf::Texture eyes_texture_;
  sf::Texture tail_texture_;
  sf::Shader color_shader_;
  sf::Texture food_density_texture_;
  sf::Shader food_density_shader_;
  sf::Texture health_texture_;
  sf::Texture energy_texture_;
  sf::Shader shader_;
  bool isDragging = false;
  sf::Vector2f initialClickPosition;
  bool isClicking = false;
  std::deque<sf::Vector2f> deltaHistory;

  sf::Vector2f offset_;

};

