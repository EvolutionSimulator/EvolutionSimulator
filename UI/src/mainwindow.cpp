#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  // Calculate total size needed for the window based on canvas size and additional widgets/controls
  int totalWidth = settings::environment::kMapWidth + 30;
  int totalHeight = settings::environment::kMapHeight + 300; // add extra height for other controls if needed

  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio
  int scaledWidth = static_cast<int>(totalWidth/scaleFactor);
  int scaledHeight = static_cast<int>(totalHeight/scaleFactor);

  // Resize the main window to fit the canvas and other controls
  resize(scaledWidth, scaledHeight);
  ui_->densityFood->setMinimum(1);
  ui_->densityFood->setMaximum(1000);
  connect(ui_->runButton, &QPushButton::clicked, this,
          &MainWindow::RunSimulation);
  connect(ui_->densityFood, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeFoodDensity(int)));
  connect(ui_->densityCreature, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeCreatureDensity(int)));
  connect(ui_->pauseButton, &QPushButton::clicked, this,
          &MainWindow::PauseSimulation);
  connect(ui_->restartButton, &QPushButton::clicked, this,
          &MainWindow::RestartSimulation);
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

void MainWindow::SetEngine(Engine *engine) {
  engine_ = engine;
  ui_->canvas->SetSimulation(engine_->GetSimulation());
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

void MainWindow::RunSimulation() {
  if (!engine_thread_.joinable()) {
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }
}

void MainWindow::PauseSimulation() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
  }
}

void MainWindow::RestartSimulation() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
  }
  // Create a new instance of the Engine and set it in the UI
  Engine *newEngine = new Engine(food_density, creature_density);
  SetEngine(newEngine);
  engine_thread_ = std::thread(&Engine::Run, engine_);
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
