#include <engine.h>

#include <QApplication>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "mainwindow.h"
#include "qsfmlcanvas.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  Engine mainEngine(width, height);

  MainWindow window;

  window.SetEngine(&mainEngine);
  window.showFullScreen();

  int result = app.exec();

  return result;
}
