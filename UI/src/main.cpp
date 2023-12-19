#include <engine.h>

#include <QApplication>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "mainwindow.h"
#include "qsfmlcanvas.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.InitializeEngine();
  window.show();

  int result = app.exec();

  return result;
}
