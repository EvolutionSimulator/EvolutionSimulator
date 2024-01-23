#include "mainwindow/graph_manager.h"
#include <set>
#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>
#include <QValueAxis>
#include <algorithm>  // Add this include for std::sort

GraphManager::GraphManager(QWidget* parent, Engine *engine, SimulationCanvas* simulationCanvas) :
    QObject(nullptr),
    parent_(parent),
    engine_(engine),
    simulationCanvas_(simulationCanvas){
    testData = {
        std::make_tuple(0.0, 0.0, 0.0), std::make_tuple(0.0, 1.0, 1.0), std::make_tuple(0.0, 2.0, 2.0), std::make_tuple(0.0, 3.0, 3.0), std::make_tuple(0.0, 4.0, 4.0),   // Series with ID 0
        std::make_tuple(1.0, 0.0, 2.0), std::make_tuple(1.0, 1.0, 4.0), std::make_tuple(1.0, 2.0, 3.0), std::make_tuple(1.0, 3.0, 7.0), std::make_tuple(1.0, 4.0, 1.0),  // Series with ID 1
        std::make_tuple(2.0, 0.0, 5.0), std::make_tuple(2.0, 1.0, 3.0), std::make_tuple(2.0, 2.0, 8.0), std::make_tuple(2.0, 3.0, 6.0), std::make_tuple(2.0, 4.0, 2.0)    // Series with ID 2
        // Add more series as needed
    };
}

void GraphManager::SetEngine(Engine* engine)
{
  engine_ = engine;
}

void GraphManager::DrawCreaturesOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureCountOverTime();
  DrawGraph(data, "Number of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesSizeOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureSizeOverTime();
  DrawGraph(data, "Average Size of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesEnergyOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureEnergyOverTime();
  DrawGraph(data, "Average Energy of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesVelocityOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureVelocityOverTime();
  DrawGraph(data, "Average Velocity of Creatures in the Simulation");
}

// Define a type alias for your data type
using DataType = std::vector<std::tuple<double, double, double>>;

bool SortByIdComparator(const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b) {
  return std::get<0>(a) < std::get<0>(b);
}

void SortById(std::vector<std::tuple<double, double, double>>& data) {
  std::sort(data.begin(), data.end(), SortByIdComparator);
}

void PrintData(const std::vector<std::tuple<double, double, double>>& data) {
  for (const auto& point : data) {
      std::cout << "ID: " << std::get<0>(point)
                << ", X: " << std::get<1>(point)
                << ", Y: " << std::get<2>(point) << "\n";
  }
  std::cout << "------\n";
}

void GraphManager::DrawAreaGraph(const std::vector<std::tuple<double, double, double>>& data, const QString& graphTitle) {
  if (data.empty()) {
      qDebug() << "No data to display.";
      return;
  }

  std::vector<std::tuple<double, double, double>> sortedData = data;
  SortById(sortedData);

  QChart* chart = new QChart();

  qreal minX = std::numeric_limits<qreal>::max();
  qreal maxX = std::numeric_limits<qreal>::min();
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = std::numeric_limits<qreal>::min();

  // Generate equally spaced colors based on the number of unique IDs
  std::set<double> uniqueIds;
  for (const auto& seriesData : data) {
      uniqueIds.insert(std::get<0>(seriesData));
  }

  const int numSeries = static_cast<int>(uniqueIds.size());
  QVector<QColor> seriesColors;

  for (double id : uniqueIds) {
      // Calculate HSV values to get equally spaced colors
      qreal hue = static_cast<qreal>(id) / numSeries;
      qreal saturation = 1.0;
      qreal value = 1.0;

      QColor color = QColor::fromHsvF(hue, saturation, value);
      seriesColors.push_back(color);
  }

  std::vector<double> lower_bound(data.size(), 0.0);

  for (double id : uniqueIds) {
      QLineSeries* upperSeries = new QLineSeries();
      QLineSeries* lowerSeries = new QLineSeries();
      for (const auto& seriesData : data) {
          if (std::get<0>(seriesData) == id) {
              qreal x = std::get<1>(seriesData); // Second element as x
              qreal y = std::get<2>(seriesData); // Third element as y

              // Append to upper series
              upperSeries->append(QPointF(x,y+lower_bound[x]));

              // Update min and max values
              minX = std::min(minX, x);
              maxX = std::max(maxX, x);
              minY = std::min(minY, y);
              maxY = std::max(maxY, y+lower_bound[x]);

              // Append to lower series
              lowerSeries->append(QPointF(x, lower_bound[x]));
              lower_bound[x] = y+lower_bound[x];
          }
      }

      QAreaSeries* areaSeries = new QAreaSeries(upperSeries, lowerSeries);
      chart->addSeries(areaSeries);

      // Set series properties, including color
      areaSeries->setName("Series " + QString::number(id));
      QPen pen(seriesColors[static_cast<int>(id)]);
      pen.setWidth(3);
      areaSeries->setPen(pen);

      QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
      gradient.setColorAt(0.0, seriesColors[static_cast<int>(id)]);
      gradient.setColorAt(1.0, seriesColors[static_cast<int>(id)].lighter(150)); // Lighter shade for gradient
      gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
      areaSeries->setBrush(gradient);
  }

  chart->setTitle(graphTitle);
  chart->createDefaultAxes();
  chart->axes(Qt::Horizontal).first()->setTitleText("Time");
  chart->axes(Qt::Vertical).first()->setTitleText("Number of Creatures");
  chart->axes(Qt::Horizontal).first()->setRange(minX, maxX);
  chart->axes(Qt::Vertical).first()->setRange(minY, maxY);


  chart->setTheme(QChart::ChartThemeLight);
  chart->setAnimationOptions(QChart::AllAnimations);
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  QVBoxLayout* layout = new QVBoxLayout();

  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(chartView);

  QPushButton* saveButton = new QPushButton("Save Graph");
  layout->addWidget(saveButton);

  QDialog* dialog = new QDialog(parent_);
  dialog->setWindowTitle(graphTitle);
  dialog->setLayout(layout);
  dialog->resize(800, 600);

  connect(saveButton, &QPushButton::clicked, [chartView, data, graphTitle, this]() {
      QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
      QString folderPath = desktopPath + "/EvolutionSimulationData/";

      if (createDirectory(folderPath)) {
          QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Graph", folderPath, "PNG Image (.png);;CSV File (.csv)");
          if (!filePath.isEmpty()) {
              // Save image
              QString imageFilePath = filePath + ".png";
              chartView->grab().save(imageFilePath);

              QString csvFilePath = filePath + ".csv";
              QFile file(csvFilePath);
              if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                  QTextStream stream(&file);
                  for (size_t id = 0; id < data.size(); ++id) {
                      auto seriesData = data[id];
                      writeTupleElements(stream, id, seriesData);
                  }
                  file.close();
              } else {
                  QMessageBox::critical(nullptr, "Error", "Failed to save CSV file.");
              }
          }
      } else {
          QMessageBox::critical(nullptr, "Error", "Failed to create the directory.");
      }
  });

  connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
  emit resetGraphMenuIndex();
  dialog->exec();
}

void GraphManager::DrawSpeciesArea() {
  DrawAreaGraph(testData, "Species Population over Time");
}

void GraphManager::DrawSizeEnergyScatterplot() {
  auto& infoPanel = simulationCanvas_->GetInfoPanel();
  Creature* selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and energy data
  std::vector<double> sizes;
  std::vector<double> energies;

  for (const auto& creature : data->creatures_) {
      float creatureSize = creature.GetSize();
      float creatureEnergy = creature.GetEnergy();

      // Check if the creature is selected
      if (selectedCreature && creature.GetID() == selectedCreature->GetID()) {
          // If selected, make size and energy negative
          sizes.push_back(-creatureSize);
          energies.push_back(-creatureEnergy);
      } else {
          // If not selected, use regular values
          sizes.push_back(creatureSize);
          energies.push_back(creatureEnergy);
      }
  }

  DrawScatterPlot(sizes, energies, "Scatterplot of Creature Size and Energy",
                  "Creature Size", "Creature Energy");
}

void GraphManager::DrawSizeVelocityScatterplot() {
  auto& infoPanel = simulationCanvas_->GetInfoPanel();
  Creature* selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and velocity data
  std::vector<double> sizes;
  std::vector<double> velocities;

  for (const auto& creature : data->creatures_) {
      float creatureSize = creature.GetSize();
      float creatureVelocity = creature.GetVelocity();

      // Check if the creature is selected
      if (selectedCreature && creature.GetID() == selectedCreature->GetID()) {
          // If selected, make size and velocity negative
          sizes.push_back(-creatureSize);
          velocities.push_back(-creatureVelocity);
      } else {
          // If not selected, use regular values
          sizes.push_back(creatureSize);
          velocities.push_back(creatureVelocity);
      }
  }

  DrawScatterPlot(sizes, velocities, "Scatterplot of Creature Size and Velocity",
                  "Creature Size", "Creature Velocity");
}

void GraphManager::DrawEnergyVelocityScatterplot() {
  auto& infoPanel = simulationCanvas_->GetInfoPanel();
  Creature* selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and velocity data
  std::vector<double> energies;
  std::vector<double> velocities;

  for (const auto& creature : data->creatures_) {
      float creatureEnergy = creature.GetEnergy();
      float creatureVelocity = creature.GetVelocity();

      // Check if the creature is selected
      if (selectedCreature && creature.GetID() == selectedCreature->GetID()) {
          // If selected, make size and velocity negative
          energies.push_back(-creatureEnergy);
          velocities.push_back(-creatureVelocity);
      } else {
          // If not selected, use regular values
          energies.push_back(creatureEnergy);
          velocities.push_back(creatureVelocity);
      }
  }

  DrawScatterPlot(energies, velocities, "Scatterplot of Creature Energy and Velocity",
                  "Creature Energy", "Creature Velocity");
}

void GraphManager::handleDropdownSelection(int index) {
  qDebug() << "Dropdown selection changed to index:" << index;

  if (index == 1) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesOverTimeGraph();
  }
  if (index == 2) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesSizeOverTimeGraph();
  }
  if (index == 3) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesEnergyOverTimeGraph();
  }
  if (index == 4) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesVelocityOverTimeGraph();
  }
  if (index == 5) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawSizeEnergyScatterplot();
  }
  if (index == 6) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawSizeVelocityScatterplot();
  }
  if (index == 7) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawEnergyVelocityScatterplot();
  }
  if (index == 8) {
    qDebug() << "Calling DrawSpeciesArea";
    DrawSpeciesArea();
  }
}
