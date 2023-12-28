#include "mainwindow.h"

#include <iostream>

#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>

#include "ui_mainwindow.h"
#include "custombutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), friction_coefficient(0.0), lastRecordedTime_(0.0) {
  ui_->setupUi(this);

  CustomButton* customRunButton = new CustomButton(this);

  // Assuming you want to replace the 'runButton' from the UI file:
  customRunButton = new CustomButton(this);
  // Connect the custom button click signal to the appropriate slot
  connect(customRunButton, &CustomButton::clicked, this, &MainWindow::ToggleSimulation);

  ui_->densityFood->setMinimum(1);
  ui_->densityFood->setMaximum(1000);
  connect(ui_->densityFood, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeFoodDensity(int)));
  connect(ui_->densityCreature, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeCreatureDensity(int)));
  connect(ui_->restartButton, &QPushButton::clicked, this,
          &MainWindow::RestartSimulation);
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
  engine_->GetEnvironment().SetFrictionalCoefficient(friction_coefficient);
  engine_->UpdateEnvironment();

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

void MainWindow::SetEngine(Engine *engine) {
  engine_ = engine;
  ui_->canvas->SetSimulation(engine_->GetSimulation());
}

void MainWindow::ChangeFoodDensity(int value) {
  food_density = static_cast<double>(value) / 1000.0;     // Convert to density
  engine_->GetEnvironment().SetFoodDensity(food_density); // Update the density
  engine_->UpdateEnvironment(); // Apply the updated density
}

void MainWindow::ChangeCreatureDensity(int value) {
  creature_density = static_cast<double>(value) / 10000.0; // Convert to density
  RestartSimulation(); // restart simulation with new creature density
}

void MainWindow::RunSimulation() {
  if (!engine_thread_.joinable()) {
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }
}

void MainWindow::ToggleSimulation() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
  } else {
    engine_thread_ = std::thread(&Engine::Run, engine_);
  }
}

void MainWindow::RestartSimulation() {
  if (engine_thread_.joinable()) {
    engine_->Stop();
    engine_thread_.join();
    delete engine_;
  }
  // Create a new instance of the Engine and set it in the UI
  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  Engine *newEngine = new Engine(width, height, food_density, creature_density);
  SetEngine(newEngine);
  engine_thread_ = std::thread(&Engine::Run, engine_);
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
  if (engine_->GetSimulation()) {
    // Get the creature count over time from the simulation data
    std::vector<int> creatureCountOverTime = engine_->GetSimulation()->GetSimulationData()->GetCreatureCountOverTime();

    // Check if there's any data to display
    if (creatureCountOverTime.empty()) {
        qDebug() << "No data to display.";
        return;
    }

    // Create a new line series
    QLineSeries *series = new QLineSeries();

    // Add data points to the series
    for (size_t i = 0; i < creatureCountOverTime.size(); ++i) {
        series->append(i, creatureCountOverTime[i]);
    }

    // Create a chart and add the series to it
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();

    // Create a chart view with the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a dialog to display the graph
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Creature Count Over Time");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);
    dialog->resize(800, 600);

    // Show the dialog modally and connect it to delete later
    connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
    dialog->exec();
  }
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
