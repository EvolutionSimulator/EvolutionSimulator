#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>

class TextureManager
{
public:
  TextureManager();

  void InitializeFile(sf::Shader &ValueSaved, std::string path);
  void InitializeFile(sf::Font &ValueSaved, std::string path);
  void InitializeFile(sf::Texture &ValueSaved, std::string path);

  // Entity textures
  sf::Texture food_texture_;
  sf::Texture egg_texture_;
  sf::Texture creature_texture_;
  sf::Texture eyes_texture_;
  sf::Texture tail_texture_;

  // Map textures
  sf::Texture texture_;
  sf::Texture food_density_texture_;

  // Info panel textures
  sf::Texture health_texture_;
  sf::Texture energy_texture_;

  // Shaders
  sf::Shader shader_;
  sf::Shader color_shader_;
  sf::Shader food_density_shader_;

  // Font
  sf::Font font_;
};
#endif // TEXTURE_MANAGER_H
