#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    connect(ui_->runButton, &QPushButton::clicked, this, &MainWindow::RunSimulation);
}

MainWindow::~MainWindow()
{
    if (engine_thread_.joinable()) {
        engine_->Stop();
        engine_thread_.join();
    }

    delete ui_;
}

void MainWindow::SetEngine(Engine *engine)
{
    engine_ = engine;
    ui_->canvas->SetSimulation(engine_->GetSimulation());
}

void MainWindow::RunSimulation()
{
    if (!engine_thread_.joinable()) {
        engine_thread_ = std::thread(&Engine::Run, engine_);
    }
}
