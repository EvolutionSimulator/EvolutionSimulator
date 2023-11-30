#pragma once

#include "engine.h"
#include <QMainWindow>
#include <thread>

QT_BEGIN_NAMESPACE                 //
namespace Ui { class MainWindow; } // <- generated by Qt, don't touch
QT_END_NAMESPACE                   //

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetEngine(Engine* engine);

private:
    void RunSimulation();
    void PauseSimulation();
    void StopSimulation();

    Ui::MainWindow* ui_; // pointer to all UI widgets (buttons, labels, texboxes, the simulation canvas, etc)

    Engine* engine_;
    std::thread engine_thread_;
};
