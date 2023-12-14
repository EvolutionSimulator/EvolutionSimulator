#include <engine.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "mainwindow.h"
#include "qsfmlcanvas.h"

#include <QApplication>
#include <thread>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Engine mainEngine;

    MainWindow window;
    window.SetEngine(&mainEngine);
    window.show();

    int result = app.exec();

    return result;
}

