#ifndef INFO_PANEL_H
#define INFO_PANEL_H

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "creature.h"
#include "qwidgets/qsfmlcanvas.h"
#include "simulation.h"
#include "texture_manager.h"

class InfoPanel {
public:

  InfoPanel(QSFMLCanvas* canvas, TextureManager* texture_manager, Simulation *simulation);

  void SetSelectedCreature(Creature* creature);
  void UpdateSelectedFood();
  Creature* GetSelectedCreature() const;

  void Show();
  void Hide();
  bool IsVisible() const;
  void Draw();
  void DrawVisionCone(sf::RenderTarget& target, const Creature &creature);
  void DrawStomach(sf::RenderTarget& target, const Creature& creature);
  std::string FormatCreatureInfo(const Creature& creature);

private:
  QSFMLCanvas* canvas_;
  Simulation* simulation_;
  Creature* selected_creature_;
  Food* selected_food_;
  bool is_visible_;
  TextureManager* texture_manager_;

  void DrawPanel(sf::RenderTarget& target);
  void DrawCreatureInfo(sf::RenderTarget& target);
};
#endif // INFO_PANEL_H
