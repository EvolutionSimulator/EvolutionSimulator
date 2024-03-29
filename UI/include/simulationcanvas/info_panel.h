#ifndef INFO_PANEL_H
#define INFO_PANEL_H

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "entity/creature/creature.h"
#include "qwidgets/qsfmlcanvas.h"
#include "simulation/simulation.h"
#include "texture_manager.h"
#include <SFML/Graphics.hpp>

class InfoPanel {
public:

  InfoPanel(QSFMLCanvas* canvas, TextureManager* texture_manager, Simulation *simulation);

  void SetSelectedCreature(std::shared_ptr<Creature> creature);
  void UpdateSelectedFood();
  std::shared_ptr<Creature> GetSelectedCreature() const;

  void Show();
  void Hide();
  bool IsVisible() const;
  void Draw();
  void DrawCircle(const Creature &creature, sf::Color color);
  void DrawVisionCone(sf::RenderTarget& target, const Creature &creature, std::pair<double, double> position);
  void DrawStomach(sf::RenderTarget& target, const Creature& creature);
  std::string FormatCreatureInfo(const Creature& creature);

  void SetUIView(sf::View view);
  void SetPanelView(sf::View view);
  void SetOffset(float offset_x, float offset_y);
  void SetSelectedSpecies(int id);
  void RemoveSelectedSpecies();
  int GetSelectedSpecies() const;

private:
  int selected_id_ = -1;

  QSFMLCanvas* canvas_;
  Simulation* simulation_;
  std::shared_ptr<Creature> selected_creature_;
  std::shared_ptr<Entity> closest_entity_;
  bool is_visible_;
  TextureManager* texture_manager_;

  void DrawPanel(sf::RenderTarget& target);
  void DrawCreatureInfo(sf::RenderTarget& target);

  sf::View ui_view_;
  sf::View info_panel_view_;

  float offset_x_;
  float offset_y_;
};
#endif // INFO_PANEL_H
