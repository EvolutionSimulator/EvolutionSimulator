#include <core/engine.h>

#include <QApplication>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "mainwindow/mainwindow.h"
#include "qwidgets/qsfmlcanvas.h"

int main(int argc, char *argv[]) {
  SETTINGS.LoadFromFile("./settings.json");

  QApplication app(argc, argv);
  MainWindow window;

#ifdef DEBUG
  std::cout << "Running in debug mode." << std::endl;
#endif

#ifdef NDEBUG
  std::cout << "Running in release mode." << std::endl;
#endif


  window.showFullScreen();

  int result = app.exec();

  return result;
}
