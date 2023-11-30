#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->densityFood->setMinimum(1);
    ui_->densityFood->setMaximum(1000);
    connect(ui_->runButton, &QPushButton::clicked, this, &MainWindow::RunSimulation);
    connect(ui_->densityFood, SIGNAL(valueChanged(int)), this, SLOT(ChangeDensity(int)));
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

void MainWindow::ChangeDensity(int value)
{
    double density = static_cast<double>(value) / 100.0; // Convert to density
    engine_->GetEnvironment().SetFoodDensity(density); // Update the density
    engine_->UpdateEnvironment(); // Apply the updated density
    std::cout << "Density changed to " << density << std::endl;
}

void MainWindow::RunSimulation()
{
    if (!engine_thread_.joinable()) {
        engine_thread_ = std::thread(&Engine::Run, engine_);
    }
}

void Nothing_but_git_testing() {
    double i = 1;
}
