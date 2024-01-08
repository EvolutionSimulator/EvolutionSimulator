#include "simulationcanvas/texture_manager.h"

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

TextureManager::TextureManager()
{
  InitializeFile(font_, ":/Resources/font.ttf");
  InitializeFile(color_shader_, ":/Shaders/colorShift.frag");
  InitializeFile(food_density_shader_, ":/Shaders/densityShader.frag");
  InitializeFile(creature_texture_, ":/Resources/Creature_base.png");
  InitializeFile(eyes_texture_, ":/Resources/Creature_eyes.png");
  InitializeFile(tail_texture_, ":/Resources/Creature_tails.png");
  InitializeFile(food_texture_, ":/Resources/Food_32x32.png");
  InitializeFile(egg_texture_, ":/Resources/Egg.png");
  InitializeFile(energy_texture_, ":/Resources/Energy.png");
  InitializeFile(health_texture_, ":/Resources/Health.png");
  InitializeFile(stomach_texture_, ":/Resources/EmptyStomach.png");
  InitializeFile(stomach_shader_, ":/Resources/stomachShader.png");
}

void TextureManager::InitializeFile(sf::Shader &ValueSaved, std::string path)
{
  QString qPath = QString::fromStdString(path);
  QFile resourceFile(qPath);
  if (!resourceFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Failed to open resource!";
    return;
  }

  QString tempFileName = "temp_" + QUuid::createUuid().toString(QUuid::WithoutBraces) + ".frag";
  QString tempFilePath = QDir::temp().absoluteFilePath(tempFileName);
  qDebug() << "Temporary file path:" << tempFilePath;

  QFile tempFile(tempFilePath);
  if (tempFile.exists())
  {
    qDebug() << "Temporary file already exists. Deleting...";
    if (!tempFile.remove())
    {
      qDebug() << "Failed to remove existing temporary file.";
      return;
    }
  }

  if (!resourceFile.copy(tempFilePath))
  {
    qDebug() << "Failed to copy to temporary file!";
    qDebug() << "Error:" << resourceFile.errorString();
    return;
  }
  resourceFile.close();

  if (!ValueSaved.loadFromFile(tempFilePath.toStdString(), sf::Shader::Fragment))
  {
    qDebug() << "Failed to load from file!";
    QFile::remove(tempFilePath);
    return;
  }

  // Clean up the temporary file after use
  QFile::remove(tempFilePath);
}

void TextureManager::InitializeFile(sf::Font &ValueSaved, std::string path)
{
  QString qPath = QString::fromStdString(path);
  QFile resourceFile(qPath);
  if (!resourceFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Failed to open resource!";
    return;
  }

  QString tempFileName = "temp_" + QUuid::createUuid().toString(QUuid::WithoutBraces) + ".frag";
  QString tempFilePath = QDir::temp().absoluteFilePath(tempFileName);
  qDebug() << "Temporary file path:" << tempFilePath;

  QFile tempFile(tempFilePath);
  if (tempFile.exists())
  {
    qDebug() << "Temporary file already exists. Deleting...";
    if (!tempFile.remove())
    {
      qDebug() << "Failed to remove existing temporary file.";
      return;
    }
  }

  if (!resourceFile.copy(tempFilePath))
  {
    qDebug() << "Failed to copy to temporary file!";
    qDebug() << "Error:" << resourceFile.errorString();
    return;
  }
  resourceFile.close();

  if (!ValueSaved.loadFromFile(tempFilePath.toStdString()))
  {
    qDebug() << "Failed to load from file!";
    QFile::remove(tempFilePath);
    return;
  }

  // Clean up the temporary file after use
  QFile::remove(tempFilePath);
}

void TextureManager::InitializeFile(sf::Texture &ValueSaved, std::string path)
{
  QPixmap creaturePixmap;
  QString qPath = QString::fromStdString(path);
  if (!creaturePixmap.load(qPath))
  {
    // qDebug() << "Failed to load QPixmap from path:" << path;
  }

  QImage creatureqImage = creaturePixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
  sf::Image creaturesfImage;
  creaturesfImage.create(creatureqImage.width(), creatureqImage.height(), reinterpret_cast<const sf::Uint8 *>(creatureqImage.bits()));

  if (!ValueSaved.loadFromImage(creaturesfImage))
  {
    qDebug() << "Failed to create sf::Texture from sf::Image";
  }
}
