#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    connect(ui_->runButton, &QPushButton::clicked, this, &MainWindow::RunSimulation);
    connect(ui_->pauseButton, &QPushButton::clicked, this, &MainWindow::PauseSimulation);
    connect(ui_->restartButton, &QPushButton::clicked, this, &MainWindow::RestartSimulation);
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

//Allows us to pause the simulation
void MainWindow::PauseSimulation()
{
    if (engine_thread_.joinable()) {
        engine_->Stop();
        engine_thread_.join();
    }
}

void MainWindow::RestartSimulation()
{
    // Stop the current simulation if running
    if (engine_thread_.joinable()) {
        engine_->Stop();
        engine_thread_.join();
    }

    // Create a new instance of the Engine and set it in the UI
    Engine* newEngine = new Engine();
    SetEngine(newEngine);

    // Start the new simulation
    engine_thread_ = std::thread(&Engine::Run, engine_);
}

void Nothing_but_git_testing() {
    double i = 1;
}
