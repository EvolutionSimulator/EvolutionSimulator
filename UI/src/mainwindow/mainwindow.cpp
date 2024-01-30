#include "mainwindow/mainwindow.h"

#include <iostream>

#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QFileDialog>

#include <QDockWidget>
#include "ui_mainwindow.h"
#include "simulationdata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), lastRecordedTime_(0.0) {
  ui_->setupUi(this);

  ui_->canvas->SetRefreshInterval(1000.0 / 60);

  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  SETTINGS.environment.map_width = width * SETTINGS.ui.max_zoom;
  SETTINGS.environment.map_height = height * SETTINGS.ui.max_zoom;

  InitializeEngine();
  PauseSimulation();
  RunSimulation();
  graph_manager_ = new GraphManager(this, engine_, cluster_, ui_->canvas);
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
    int width = SETTINGS.environment.map_width;
    int height = SETTINGS.environment.map_height;
    engine_ = new Engine(width, height);
    engine_->SetSpeed(100);
    ui_->canvas->SetSimulation(engine_->GetSimulation());

    cluster_ = new Cluster(SETTINGS.compatibility.compatibility_threshold * 3/4, 10);
  }

  // If this function changes change the kMaxFoodDensityColor in config.h as for a correct shade of the backgroung we need this measure
  auto food_density_function = [this](double x, double y) {
    auto data = engine_->GetSimulation()->GetSimulationData();
    return x * y * 2 / (SETTINGS.environment.map_width *
                        SETTINGS.environment.map_height) * 5e-5;
  };

  auto data = engine_->GetSimulation()->GetSimulationData();

  data->GetEnvironment().SetFoodDensity(food_density_function); // Update the density
  //ui_->canvas->UpdateFoodDensityTexture(*data);
}


void MainWindow::InitializeEngineWithDensities(double food_density, double creature_density)
{
  if (!engine_) {
    std::cout << "Creating new engine..." << std::endl;
    int width = SETTINGS.environment.map_width;
    int height = SETTINGS.environment.map_height;
    engine_ = new Engine(width, height, food_density, creature_density);
    ui_->canvas->SetSimulation(engine_->GetSimulation());
  }
}

Cluster* MainWindow::GetCluster() {
  return cluster_;
}

void MainWindow::RunSimulation() {
  if (engine_ && !engine_thread_.joinable()) {
    std::cout << "Starting engine on a separate thread..." << std::endl;
    engine_thread_ = std::thread(&Engine::Run, engine_);

    std::cout << "Starting cluster on a separate thread..." << std::endl;
    cluster_thread_ =
        std::thread(&Cluster::start, cluster_, engine_->GetSimulation());
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
    std::cout << "Stopping cluster..." << std::endl;
    cluster_->stop();
    std::cout << "Cluster stopped." << std::endl;

    std::cout << "Stopping engine..." << std::endl;
    engine_->Stop();
    std::cout << "Engine stopped." << std::endl;
  }

  if (cluster_thread_.joinable()) {
    std::cout << "Joining cluster thread..." << std::endl;
    cluster_thread_.join();
  }

  if (engine_thread_.joinable()) {
    std::cout << "Joining engine thread..." << std::endl;
    engine_thread_.join();
  }

  std::cout << "Deleting cluster..." << std::endl;
  delete cluster_;
  std::cout << "Cluster deleted." << std::endl;

  std::cout << "Deleting engine..." << std::endl;
  delete engine_;
  std::cout << "Engine deleted." << std::endl;

  engine_ = nullptr;
  cluster_ = nullptr;
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
    
    auto cluster_data = cluster_->getCurrentSpeciesData();
    for (const auto& sample : cluster_data) {
      std::cout << "ID: " << std::get<0>(sample)
                << "; time: " << std::get<1>(sample)
                << "; size: " << std::get<2>(sample) << std::endl;
    }
  }
}



void MainWindow::RestartSimulation() {
  KillEngine();
  InitializeEngine();
  // Point graphs and config to new engine
  graph_manager_->SetEngine(engine_);
  config_manager_->SetEngine(engine_);
  // Start new simulation
  PauseSimulation();
  RunSimulation();
  // De-select creature in info panel
  ui_->canvas->GetInfoPanel().Hide();
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

void MainWindow::SetUpConnections()
{
    // Connect configuration menu signals
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
    comboBox->addItem("Creatures Energy Over Time");
    comboBox->addItem("Creatures Velocity Over Time");
    comboBox->addItem("Creatures Diet Over Time");
    comboBox->addItem("Creatures Offspring Over Time");
    comboBox->addItem("Size and Energy Scatterplot");
    comboBox->addItem("Size and Velocity Scatterplot");
    comboBox->addItem("Energy and Velocity Scatterplot");
    comboBox->addItem("Species Area");
    connect(graph_manager_, &GraphManager::resetGraphMenuIndex, this, [this]() {
        ui_->graphMenu->setCurrentIndex(0);
    });

    // Draw save menu
    QComboBox *comboBox2 = ui_->saveMenu;
    QPixmap originalPixmap2(":/Resources/save.png");
    QPixmap scaledPixmap2 = originalPixmap2.scaled(20, 20, Qt::KeepAspectRatio);
    comboBox2->setIconSize(scaledPixmap2.size());

    comboBox2->addItem(QIcon(scaledPixmap2), "Load / Save");
    comboBox2->addItem("Load Data");
    comboBox2->addItem("Save Data");
    connect(ui_->saveMenu, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, &MainWindow::handleDropdownSelectionSave);

    // Set up run, restart
    QRect rect(2,2,45,45);
    QRect rect3(2,2,45,45);
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

    //Set up config buttons new colors
    ui_->configurationButton->setMask(region);
    QPixmap pixMap2(":/Resources/Configuration.png");
    QIcon icon2(pixMap2);
    ui_->configurationButton->setIcon(icon2);
    QSize size(50, 50);

    ui_->runButton->setIconSize(size);
    ui_->configurationButton->setIconSize(size);
    ui_->restartButton->setIconSize(size);
}

void MainWindow::handleDropdownSelectionSave(int index) {
  qDebug() << "Dropdown selection changed to index:" << index;

  if (index == 1) {
      emit PauseSimulation();
      qDebug() << "Load Button";

      QString dir = QFileDialog::getOpenFileName(this, "Open file", "", "JSON Files (*.json)");

      if (dir.isNull() || dir.isEmpty()) {
          qDebug() << "Load canceled by user";
          emit ResumeSimulation();
          return;  // Return without attempting to load data
      }

      std::filesystem::path filePath = std::filesystem::path(dir.toStdString());
      if (std::filesystem::exists(filePath)) {
          engine_->GetSimulation()->GetSimulationData()->RetrieveDataFromFile(filePath);
          qDebug() << "Load successful";
      } else {
          qDebug() << "File does not exist: " << dir;
      }

      emit ResumeSimulation();
  }
  if (index == 2) {
    emit PauseSimulation();
    qDebug() << "Save button";
    // save the data to a new file
    SimulationData data = *engine_->GetSimulation()->GetSimulationData();
    qDebug() << "Saving simulation";
    std::filesystem::path currentFilePath = __FILE__;
    std::filesystem::path evolutionSimulatorPath = currentFilePath.parent_path().parent_path().parent_path();
    std::filesystem::path file_path = evolutionSimulatorPath / "Simulations";
    std::filesystem::create_directory(file_path);
    QString dir = QFileDialog::getSaveFileName(this, "Save as", QString::fromStdString(file_path.string()));
    if (dir.isNull() || dir.isEmpty()) {
            qDebug() << "Save canceled by user";
            emit ResumeSimulation();
            return;  // Return without attempting to save data
        }
    std::filesystem::path saveFilePath = std::filesystem::path(dir.toStdString());
    data.WriteDataToFile(saveFilePath);


    // Save the statistics to a separate file
    QFileInfo fileInfo(dir);
    QString fileName = fileInfo.fileName();
    QRegularExpression regex("^[a-zA-Z0-9_()]+$");
        if (!regex.match(fileName).hasMatch()) {
            qDebug() << "Invalid characters in file name";
            emit ResumeSimulation();
            return;
        }
    QString simulationFilePath = QString("Statistics(") + fileName + QString(")");
    qDebug() << simulationFilePath;
    std::filesystem::path statisticsFilePath = file_path / simulationFilePath.toStdString();
    data.WriteStatisticsToFile(statisticsFilePath);

    qDebug() << "Saved simulation";
    emit ResumeSimulation();
  }
  ui_->saveMenu->setCurrentIndex(0);
}
