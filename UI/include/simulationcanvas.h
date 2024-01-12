#pragma once

#include <engine.h>

#include <SFML/Graphics.hpp>

#include "qsfmlcanvas.h"
#include "simulation.h"

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
     int food_id = -1;
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

  void zoom(float factor, sf::Vector2f& zoomPoint);

  void InitializeFont();
  void InitializeSprites();
  void InitializeShader();

  protected:
  void mousePressEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

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

  Simulation *simulation_ = nullptr;
  std::function<void(SimulationData *)>
      render_lambda_; // we will pass this to the ProcessData method of
                      // Simulation

  bool isCreatureClicked(const sf::Vector2f &mousePos);

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
  sf::Shader shader_;
  bool isDragging = false;
  sf::Vector2f initialClickPosition;
  bool isClicking = false;
  std::deque<sf::Vector2f> deltaHistory;
};

sf::Sprite spriteTexture(double size);
