#include <core/engine.h>

#include <QApplication>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // SETTINGS.LoadFromFile("./settings.json");

  MainWindow window;
  window.InitializeEngine();
  window.show();

  int result = app.exec();

  return result;
}
