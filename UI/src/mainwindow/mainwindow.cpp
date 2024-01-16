#include "mainwindow/mainwindow.h"

#include <iostream>

#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include "QtWidgets/qslider.h"

#include <QDockWidget>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), lastRecordedTime_(0.0) {
  ui_->setupUi(this);

  InitializeEngine();
  PauseSimulation();
  RunSimulation();

  graph_manager_ = new GraphManager(this, engine_);
  config_manager_ = new ConfigManager(this, engine_);

  DrawUI();
  SetUpConnections();

}



MainWindow::~MainWindow() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
  }
  
  delete ui_;
}

void MainWindow::InitializeEngine()
{
  if (!engine_) {
    std::cout << "Creating new engine..." << std::endl;
    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;
    engine_ = new Engine(width, height);
    ui_->canvas->SetSimulation(engine_->GetSimulation());
  }
  // If this function changes change the kMaxFoodDensityColor in config.h as for a correct shade of the backgroung we need this measure
  auto food_density_function = [this](double x, double y) {
    return x * y * 2 / (ui_->canvas->GetSimulation()->GetSimulationData()->GetEnvironment().GetMapWidth() *
                        ui_->canvas->GetSimulation()->GetSimulationData()->GetEnvironment().GetMapHeight()) * 5e-5;
  };
  engine_->GetEnvironment().SetFoodDensity(food_density_function); // Update the density
  engine_->GetSimulation()->GetSimulationData()->InitializeFood();
  ui_->canvas->UpdateFoodDensityTexture(engine_->GetEnvironment().GetMapWidth(),
                                        engine_->GetEnvironment().GetMapHeight());
}


void MainWindow::InitializeEngineWithDensities(double food_density, double creature_density)
{
  if (!engine_) {
    std::cout << "Creating new engine..." << std::endl;
    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;
    engine_ = new Engine(width, height, food_density, creature_density);
    ui_->canvas->SetSimulation(engine_->GetSimulation());
  }
}


void MainWindow::RunSimulation() {
  if (engine_ && !engine_thread_.joinable()) {
    std::cout << "Starting engine on a separate thread..." << std::endl;
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }
}

void MainWindow::PauseSimulation()
{
  if (engine_) {
    engine_->Pause();
  }
  QPixmap pixMap(":/Resources/Run.png");
  QIcon icon(pixMap);
  ui_->runButton->setIcon(icon);
}

void MainWindow::ResumeSimulation()
{
  if (engine_) {
    engine_->Resume();
  }
  QPixmap pixMap(":/Resources/Pause.png");
  QIcon icon(pixMap);
  ui_->runButton->setIcon(icon);
}

void MainWindow::KillEngine()
{
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

void MainWindow::ToggleSimulation() {
  if (engine_->IsPaused()) {
    engine_->Resume();
    //change icon of button
    QPixmap pixMap2(":/Resources/Pause.png");
    QIcon icon2(pixMap2);
    ui_->runButton->setIcon(icon2);
  } else {
    engine_->Pause();
    //change icon of button
    QPixmap pixMap2(":/Resources/Run.png");
    QIcon icon2(pixMap2);
    ui_->runButton->setIcon(icon2);
  }
}



void MainWindow::RestartSimulation() {
  KillEngine();
  InitializeEngine();
  RunSimulation();
}


void MainWindow::handleRestartSimulationRequested(double food_density, double creature_density) {
  KillEngine();
  InitializeEngineWithDensities(food_density, creature_density);
  RunSimulation();
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (isFullScreen()) {
            showMaximized();  // Switch to maximized mode
        } else {
            showFullScreen(); // Optionally, toggle back to full screen
        }
    } else {
        QMainWindow::keyPressEvent(event); // Pass other events to the base class
    }
}


void MainWindow::handleUIUpdateForConfigScreen(double food_density, double friction_coefficient){
    ui_->frictionLabel->setText(QString::number(friction_coefficient, 'f', 2));
}

void MainWindow::SetUpConnections()
{
    // Connect configuration menu signals
    connect(config_manager_, &ConfigManager::UpdateUIForConfigScreen, this, &MainWindow::handleUIUpdateForConfigScreen);
    connect(config_manager_, &ConfigManager::RestartSimulationRequested, this, &MainWindow::handleRestartSimulationRequested);
    connect(config_manager_, &ConfigManager::PauseSimulation, this, &MainWindow::PauseSimulation);
    connect(config_manager_, &ConfigManager::ResumeSimulation, this, &MainWindow::ResumeSimulation);

    // Connect buttons
    connect(ui_->runButton, &QPushButton::clicked, this,
            &MainWindow::ToggleSimulation);
    connect(ui_->restartButton, &QPushButton::clicked, this,
            &MainWindow::RestartSimulation);
    connect(ui_->configurationButton, &QPushButton::clicked, config_manager_,
            &ConfigManager::ShowConfigScreen);

    // Connect graph logic
    connect(ui_->graphMenu, SIGNAL(currentIndexChanged(int)), graph_manager_, SLOT(handleDropdownSelection(int)));
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(recordCreatureCount()));
    updateTimer->start(1000);  // Set the interval to 1000 milliseconds (1 second)

    // Connect friction slider on main UI
    connect(ui_->frictionCoefficientSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(ChangeFrictionCoefficient(double)));
    connect(ui_->frictionCoefficientSpinBox, &QSlider::valueChanged, config_manager_, &ConfigManager::ChangeFriction);
    ui_ -> frictionCoefficientSpinBox->setTracking(true);

}

void MainWindow::DrawUI()
{
    // Draw graph menu
    QComboBox *comboBox = ui_->graphMenu;
    QPixmap originalPixmap(":/Resources/graph-wiki_ver_1.png");
    QPixmap scaledPixmap = originalPixmap.scaled(20, 20, Qt::KeepAspectRatio);
    comboBox->setIconSize(scaledPixmap.size());
    comboBox->addItem(QIcon(scaledPixmap), "Graphs");
    comboBox->addItem("Number of Creatures Over Time");
    comboBox->addItem("Creatures Size Over Time");

    // Set up run, restart
    QRect rect(2,2,45,45);
    QRect rect3(2,2,46,45);
    QRegion region(rect, QRegion::Ellipse);
    QRegion region3(rect3, QRegion::Ellipse);
    ui_->runButton->setMask(region);
    ui_->restartButton->setMask(region3);
    QPixmap pixMap(":/Resources/Run.png");
    QPixmap pixMap3(":/Resources/Restart.png");
    QIcon icon(pixMap);
    QIcon icon3(pixMap3);
    ui_->runButton->setIcon(icon);
    ui_->restartButton->setIcon(icon3);

    //Set up config buttons
    ui_->configurationButton->setMask(region);
    QPixmap pixMap2(":/Resources/Config-settings.png");
    QIcon icon2(pixMap2);
    ui_->configurationButton->setIcon(icon2);
    QSize size(50, 50);

    ui_->runButton->setIconSize(size);
    ui_->configurationButton->setIconSize(size);
    ui_->restartButton->setIconSize(size);
}
