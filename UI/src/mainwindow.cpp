#include "mainwindow.h"

#include <iostream>

#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), engine_(nullptr), friction_coefficient(0.0), lastRecordedTime_(0.0) {
  ui_->setupUi(this);

  InitializeEngine();
  RunEngine();
  PauseEngine();

  // Calculate total size needed for the window based on canvas size and additional widgets/controls
  int totalWidth = SETTINGS.environment.map_width + 30;
  int totalHeight = SETTINGS.environment.map_height + 300; // add extra height for other controls if needed

  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio
  int scaledWidth = static_cast<int>(totalWidth/scaleFactor);
  int scaledHeight = static_cast<int>(totalHeight/scaleFactor);

  // Resize the main window to fit the canvas and other controls
  resize(scaledWidth, scaledHeight);

  // connect(ui_->densityFood, SIGNAL(valueChanged(int)), this,
  //        SLOT(ChangeFoodDensity(int)));
  // connect(ui_->densityCreature, SIGNAL(valueChanged(int)), this,
  //        SLOT(ChangeCreatureDensity(int)));
  connect(ui_->restartButton, &QPushButton::clicked, this,
          &MainWindow::RestartEngine);
  connect(ui_->pauseButton, &QPushButton::clicked, this,
          &MainWindow::PauseEngine);
  connect(ui_->resumeButton, &QPushButton::clicked, this,
          &MainWindow::ResumeEngine);
  connect(ui_->graphButton, &QPushButton::clicked, this,
          &MainWindow::DrawCreaturesOverTimeGraph);
  updateTimer = new QTimer(this);
  connect(updateTimer, SIGNAL(timeout()), this, SLOT(recordCreatureCount()));
  updateTimer->start(1000);  // Set the interval to 1000 milliseconds (1 second)
  connect(ui_->frictionCoefficientSpinBox, SIGNAL(valueChanged(double)), this,
          SLOT(ChangeFrictionCoefficient(double)));
  connect(ui_->frictionCoefficientSpinBox, &QSlider::valueChanged, this, &MainWindow::ChangeFriction);
}

void MainWindow::ChangeFriction(int value) {
  double friction_coefficient = static_cast<double>(value) / 100.0;  // Scale slider value to be in the range 0.05 to 0.20
  engine_->GetSimulation()->GetEnvironment()->SetFrictionalCoefficient(friction_coefficient);

  // Update the QLabel to display the current value
  ui_->frictionLabel->setText(QString::number(friction_coefficient, 'f', 2));  // Display with 2 decimal places
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

  auto environment = engine_->GetSimulation()->GetEnvironment();
  environment->SetFoodDensity(food_density);  // Update the density
}

void MainWindow::ChangeCreatureDensity(int value) {
  creature_density =
      static_cast<double>(value) / 10000.0;  // Convert to density
  RestartEngine();  // restart simulation with new creature density
}

void MainWindow::InitializeEngine() {
  if (!engine_) {
    std::cout << "Creating new engine..." << std::endl;
    engine_ = new Engine();
    ui_->canvas->SetSimulation(engine_->GetSimulation());
  }
}

void MainWindow::RunEngine() {
  if (engine_) {
    if (!engine_thread_.joinable()) {
      std::cout << "Starting engine on a separate thread..." << std::endl;
      engine_thread_ = std::thread(&Engine::Run, engine_);
    } else {
      engine_->Resume();
    }
  }
}

void MainWindow::RestartEngine() {
  KillEngine();

  InitializeEngine();

  RunEngine();
}

void MainWindow::PauseEngine() {
  if (engine_) {
    engine_->Pause();
  }
}

void MainWindow::ResumeEngine() {
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
  } else {
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }

  std::cout << "Deleting engine..." << std::endl;
  delete engine_;
  std::cout << "Engine deleted." << std::endl;
  engine_ = nullptr;
}

double ExampleGraphFunction(double x) { return x * x; }

// This function is not complete, it is just an example of how to display a
// graph
void MainWindow::DisplayGraph() {
  // Create a new line series
  QLineSeries *series = new QLineSeries();

  // Add data points to the series
  for (double x = 0.0; x <= 10.0; x += 1.0) {
    double y = ExampleGraphFunction(x);
    series->append(x, y);
  }

  // Create a chart and add the series to it
  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->createDefaultAxes();

  // Create a chart view with the chart
  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  QDialog *dialog = new QDialog(this); // 'this' sets MainWindow as the parent
  dialog->setWindowTitle("Graph Display");
  QVBoxLayout *layout = new QVBoxLayout(dialog);
  layout->addWidget(chartView);
  dialog->setLayout(layout);
  dialog->resize(800, 600);

  dialog->show();
}


void MainWindow::DrawCreaturesOverTimeGraph() {
  // if (engine_->GetSimulation()) {
  //   // Get the creature count over time from the simulation data
  //   std::vector<int> creatureCountOverTime = engine_->GetSimulation()->GetSimulationData()->GetCreatureCountOverTime();

  //   // Check if there's any data to display
  //   if (creatureCountOverTime.empty()) {
  //       qDebug() << "No data to display.";
  //       return;
  //   }

  //   // Create a new line series
  //   QLineSeries *series = new QLineSeries();

  //   auto data = engine_->GetSimulation()->GetSimulationData();
  //   // Pass relevant information to the drawing function directly
  //   ui_->canvas->DrawCreatureCountOverTime(graphWindow, data->creatures_);

  //   // Add data points to the series
  //   for (size_t i = 0; i < creatureCountOverTime.size(); ++i) {
  //       series->append(i, creatureCountOverTime[i]);
  //   }


  //   // Create a chart and add the series to it
  //   QChart *chart = new QChart();
  //   chart->addSeries(series);
  //   chart->createDefaultAxes();

  //   // Create a chart view with the chart
  //   QChartView *chartView = new QChartView(chart);
  //   chartView->setRenderHint(QPainter::Antialiasing);

  //   // Create a dialog to display the graph
  //   QDialog *dialog = new QDialog(this);
  //   dialog->setWindowTitle("Creature Count Over Time");
  //   QVBoxLayout *layout = new QVBoxLayout(dialog);
  //   layout->addWidget(chartView);
  //   dialog->setLayout(layout);
  //   dialog->resize(800, 600);

  //   // Show the dialog modally and connect it to delete later
  //   connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
  //   dialog->exec();
  // }
}
