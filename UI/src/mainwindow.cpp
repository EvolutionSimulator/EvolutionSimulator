#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), engine_(nullptr) {
  ui_->setupUi(this);

  InitializeEngine();

  //  connect(ui_->densityFood, SIGNAL(valueChanged(int)), this,
  //          SLOT(ChangeFoodDensity(int)));
  //  connect(ui_->densityCreature, SIGNAL(valueChanged(int)), this,
  //          SLOT(ChangeCreatureDensity(int)));

  connect(ui_->runButton, &QPushButton::clicked, this,
          &MainWindow::RunSimulation);
  connect(ui_->restartButton, &QPushButton::clicked, this,
          &MainWindow::RestartSimulation);
  connect(ui_->pauseButton, &QPushButton::clicked, this,
          &MainWindow::PauseSimulation);
  connect(ui_->resumeButton, &QPushButton::clicked, this,
          &MainWindow::ResumeSimulation);
  connect(ui_->graphButton, &QPushButton::clicked, this,
          &MainWindow::DisplayGraph);
  connect(ui_->testerButton, &QPushButton::clicked, this,
          &MainWindow::GraphExampleFunction);
}

MainWindow::~MainWindow() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
  }

  delete ui_;
}

void MainWindow::ChangeFoodDensity(int value) {
  food_density = static_cast<double>(value) / 1000.0;      // Convert to density
  engine_->GetEnvironment().SetFoodDensity(food_density);  // Update the density
  engine_->UpdateEnvironment();  // Apply the updated density
}

void MainWindow::ChangeCreatureDensity(int value) {
  creature_density =
      static_cast<double>(value) / 10000.0;  // Convert to density
  RestartSimulation();  // restart simulation with new creature density
}

void MainWindow::InitializeEngine() {
  if (!engine_) {
    std::cout << "Creating new engine..." << std::endl;
    engine_ = new Engine();
    ui_->canvas->SetSimulation(engine_->GetSimulation());
  }
}

void MainWindow::RunSimulation() {
  if (engine_ && !engine_thread_.joinable()) {
    std::cout << "Starting engine on a separate thread..." << std::endl;
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }
}

void MainWindow::RestartSimulation() {
  KillEngine();

  InitializeEngine();

  RunSimulation();
}

void MainWindow::PauseSimulation() {
  if (engine_) {
    engine_->Pause();
  }
}

void MainWindow::ResumeSimulation() {
  if (engine_) {
    engine_->Resume();
  }
}

void MainWindow::KillEngine() {
  if (engine_) {
    std::cout << "Stopping engine..." << std::endl;
    engine_->Stop();
  }

  if (engine_thread_.joinable()) {
    std::cout << "Joining engine thread..." << std::endl;
    engine_thread_.join();
  }

  std::cout << "Deleting engine..." << std::endl;
  delete engine_;
  std::cout << "Engine deleted." << std::endl;
  engine_ = nullptr;
}

double ExampleGraphFunction(double x) { return x * x; }

void MainWindow::DisplayGraph() {
  sf::RenderWindow graphWindow(sf::VideoMode(480, 360), "Graph Window");

  while (graphWindow.isOpen()) {
    sf::Event event;
    while (graphWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        graphWindow.close();
      }
    }

    graphWindow.clear(sf::Color::White);

    // Pass relevant information to the drawing function directly
    ui_->canvas->DrawCreatureCountOverTime(
        graphWindow, engine_->GetSimulation()->GetSimulationData()->creatures_);

    graphWindow.display();
  }
}

void MainWindow::GraphExampleFunction() {
  // Code to graph the ExampleGraphFunction(x)
  // You can use a plotting library or custom code to draw the graph
  // For simplicity, let's print the values to the console for now
  for (double x = 0.0; x <= 10.0; x += 1.0) {
    double result = ExampleGraphFunction(x);
    std::cout << "x: " << x << ", f(x): " << result << std::endl;
  }
}
