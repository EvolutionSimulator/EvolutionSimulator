#include "mainwindow.h"

#include <iostream>

#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include "QtWidgets/qslider.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), friction_coefficient(0.0), lastRecordedTime_(0.0) {
  ui_->setupUi(this);

  InitializeEngine();
  PauseSimulation();
  RunSimulation();

  // Assuming you have a QComboBox named 'yourComboBoxName' in your UI
  QComboBox *comboBox = ui_->graphMenu;

  // Load the original image
  QPixmap originalPixmap(":/Resources/graph-wiki_ver_1.png");

  // Scale the image to a desired size (e.g., 20x20 pixels)
  QPixmap scaledPixmap = originalPixmap.scaled(20, 20, Qt::KeepAspectRatio);

  // Set the scaled image as the down arrow icon
  comboBox->setIconSize(scaledPixmap.size());
  comboBox->addItem(QIcon(scaledPixmap), "Graphs");
  comboBox->addItem("# Creatures Over Time");
  comboBox->addItem("Creatures Size Over Time");
  // Add more items as needed

  //Add image as icon use region as mask to make the icon circular
  QRect rect(2,2,45,45);
  QRect rect3(2,2,46,45);
  qDebug() << rect.size();
  qDebug() << ui_->runButton->size();
  QRegion region(rect, QRegion::Ellipse);
  QRegion region3(rect3, QRegion::Ellipse);
  qDebug() << region.boundingRect().size();
  ui_->runButton->setMask(region);
  ui_->restartButton->setMask(region3);
  QPixmap pixMap(":/Resources/Run.png");
  QPixmap pixMap3(":/Resources/Restart.png");
  QIcon icon(pixMap);
  QIcon icon3(pixMap3);
  ui_->runButton->setIcon(icon);
  ui_->restartButton->setIcon(icon3);
  QSize size(50, 50);
  ui_->runButton->setIconSize(size);
  ui_->restartButton->setIconSize(size);
  connect(ui_->runButton, &QPushButton::clicked, this,
          &MainWindow::ToggleSimulation);
  ui_->densityFood->setMinimum(1);
  ui_->densityFood->setMaximum(1000);
  connect(ui_->densityFood, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeFoodDensity(int)));
  connect(ui_->densityCreature, SIGNAL(valueChanged(int)), this,
          SLOT(ChangeCreatureDensity(int)));
  connect(ui_->restartButton, &QPushButton::clicked, this,
          &MainWindow::RestartSimulation);
  connect(ui_->graphMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownSelection(int)));
  updateTimer = new QTimer(this);
  connect(updateTimer, SIGNAL(timeout()), this, SLOT(recordCreatureCount()));
  updateTimer->start(1000);  // Set the interval to 1000 milliseconds (1 second)
  connect(ui_->frictionCoefficientSpinBox, SIGNAL(valueChanged(double)), this,
          SLOT(ChangeFrictionCoefficient(double)));
  connect(ui_->frictionCoefficientSpinBox, &QSlider::valueChanged, this, &MainWindow::ChangeFriction);

  ui_ -> frictionCoefficientSpinBox->setTracking(true);
}

void MainWindow::ChangeFriction(int value) {
  double friction_coefficient = static_cast<double>(value) / 100.0;  // Scale slider value to be in the range 0.05 to 0.20
  engine_->GetEnvironment().SetFrictionalCoefficient(friction_coefficient);
  engine_->UpdateEnvironment();

  // Update the QLabel to display the current value
  ui_->frictionLabel->setText(QString::number(friction_coefficient, 'f', 2));  // Display with 2 decimal places
  //ui_->frictionCoefficientSpinBox->setVisible(true);

  //ui_->frictionCoefficientSpinBox->update();
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
    //engine_->SetSpeed(10);
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

void MainWindow::ChangeFoodDensity(int value) {
  food_density = static_cast<double>(value) / 100000.0;     // Convert to density
  auto food_density_function = [this](double x, double y) {
      return x * y * 2 / (ui_->canvas->GetSimulation()->GetSimulationData()->GetEnvironment().GetMapWidth() *
                          ui_->canvas->GetSimulation()->GetSimulationData()->GetEnvironment().GetMapHeight()) * food_density;
  };
  engine_->GetEnvironment().SetFoodDensity(food_density_function); // Update the density
  engine_->UpdateEnvironment(); // Apply the updated density
  ui_->canvas->UpdateFoodDensityTexture(engine_->GetEnvironment().GetMapWidth(),
                                        engine_->GetEnvironment().GetMapHeight());
}

void MainWindow::ChangeCreatureDensity(int value) {
  creature_density = static_cast<double>(value) / 10000.0; // Convert to density
  RestartSimulation(); // restart simulation with new creature density
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
}

void MainWindow::ResumeSimulation()
{
  if (engine_) {
    engine_->Resume();
  }
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
    chart->axes(Qt::Horizontal).first()->setTitleText("Time Elapsed");
    chart->axes(Qt::Vertical).first()->setTitleText("Number of Creatures in the Simulation");

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
void MainWindow::DrawCreaturesSizeOverTimeGraph() {
  if (engine_->GetSimulation()) {
    // Get the creature count over time from the simulation data
    std::vector<int> creatureCountOverTime = engine_->GetSimulation()->GetSimulationData()->GetCreatureSizeOverTime();

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
    chart->axes(Qt::Horizontal).first()->setTitleText("Time Elapsed");
    chart->axes(Qt::Vertical).first()->setTitleText("Average Size of Creatures in the Simulation");

    // Create a chart view with the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a dialog to display the graph
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Average Creature Size Over Time");
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

void MainWindow::handleDropdownSelection(int index) {
    // Here, you can call the function corresponding to the selected item
    qDebug() << "Dropdown selection changed to index:" << index;

    // Check if the selected index is 1, then call DrawCreaturesOverTimeGraph
    if (index == 1) {
        qDebug() << "Calling DrawCreaturesOverTimeGraph";
        DrawCreaturesOverTimeGraph();
    }
    if (index == 2) {
        qDebug() << "Calling DrawCreaturesOverTimeGraph";
        DrawCreaturesSizeOverTimeGraph();
    }
}

void MainWindow::ClearResidue() {
    if (ui_->frictionCoefficientSpinBox) {
        // Remove the old slider widget
        QWidget* oldSlider = ui_->frictionCoefficientSpinBox->findChild<QWidget*>("qt_slider_handle");
        if (oldSlider) {
            oldSlider->setVisible(false);
            delete oldSlider;
        }

        // Hide only the horizontal slider handle
        QString styleSheet = "QSlider::handle:horizontal { width: 0px; }";
        ui_->frictionCoefficientSpinBox->setStyleSheet(styleSheet);

        // Force a repaint for the slider widget
        ui_->frictionCoefficientSpinBox->update();
    }
}

void MainWindow::on_frictionCoefficientSpinBox_valueChanged(int value)
{
    ui_->frictionCoefficientSpinBox->update();
}


void MainWindow::on_frictionCoefficientSpinBox_sliderMoved(int position)
{
    ui_->frictionCoefficientSpinBox->update();
}

