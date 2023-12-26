#include <engine.h>

#include <QApplication>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "mainwindow.h"
#include "qsfmlcanvas.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Engine mainEngine;

  MainWindow window;
  window.SetEngine(&mainEngine);
  window.showMaximized();

  int result = app.exec();

  return result;
}
