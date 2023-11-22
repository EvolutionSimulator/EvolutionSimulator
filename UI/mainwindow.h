#pragma once

#include "engine.h"
#include <QMainWindow>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetEngine(Engine* engine);

private:
    void RunSimulation();
    void StopSimulation();

    Ui::MainWindow* ui_;

    Engine* engine_;
    std::thread engine_thread_;
};
