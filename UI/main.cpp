#include <engine.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "mainwindow.h"
#include "qsfmlcanvas.h"

#include <QApplication>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Engine mainEngine;

    MainWindow w;
    w.SetEngine(&mainEngine);
    w.show();

    int result = a.exec();

    return result;
}
