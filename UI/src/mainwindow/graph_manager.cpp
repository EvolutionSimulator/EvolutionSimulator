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
#include <QToolTip>
#include <algorithm>  // Add this include for std::sort

GraphManager::GraphManager(QWidget *parent, Engine *engine, Cluster *cluster,
                           SimulationCanvas *simulationCanvas)
    : QObject(nullptr), parent_(parent), engine_(engine), cluster_(cluster),
      simulationCanvas_(simulationCanvas) {
  testData = {
      std::make_tuple(0.0, 0.0, 0.0), std::make_tuple(0.0, 1.0, 1.0),
      std::make_tuple(0.0, 2.0, 2.0), std::make_tuple(0.0, 3.0, 3.0),
      std::make_tuple(0.0, 4.0, 4.0), // Series with ID 0
      std::make_tuple(1.0, 0.0, 2.0), std::make_tuple(1.0, 1.0, 4.0),
      std::make_tuple(1.0, 2.0, 3.0), std::make_tuple(1.0, 3.0, 7.0),
      std::make_tuple(1.0, 4.0, 1.0), // Series with ID 1
      std::make_tuple(2.0, 0.0, 5.0), std::make_tuple(2.0, 1.0, 3.0),
      std::make_tuple(2.0, 2.0, 8.0), std::make_tuple(2.0, 3.0, 6.0),
      std::make_tuple(2.0, 4.0, 2.0) // Series with ID 2
                                     // Add more series as needed
  };
}

void GraphManager::SetEngine(Engine* engine)
{
  engine_ = engine;
}

void GraphManager::DrawCreaturesOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureCountOverTime(), "Number of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesSizeOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureSizeOverTime(), "Average Size of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesEnergyOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureEnergyOverTime(), "Average Energy of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesVelocityOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureVelocityOverTime(), "Average Velocity of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesDietOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureDietOverTime(), "Average diet of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesOffspringOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData();
  DrawGraph(data->GetCreatureOffspringOverTime(), "Average children of Creatures in the Simulation");
}

bool SortByIdComparator(const std::tuple<int, double, int, float>& a, const std::tuple<int, double, int, float>& b) {
  if (std::get<0>(a) < std::get<0>(b)) return 1;
  else{
    if (std::get<0>(a) > std::get<0>(b)) return 0;
    else {return std::get<1>(a) < std::get<1>(b);}
  }
}

bool SortByDateComparator(const std::tuple<int, double, int, float>& a, const std::tuple<int, double, int, float>& b) {
  return std::get<1>(a) < std::get<1>(b);
}

void SortById(std::vector<std::tuple<int, double, int, float>>& data) {
  std::sort(data.begin(), data.end(), SortByIdComparator);
}

void SortByDate(std::vector<std::tuple<int, double, int, float>>& data) {
  std::sort(data.begin(), data.end(), SortByDateComparator);
}


void PrintData(const std::vector<std::tuple<int, double, int, float>>& data) {
  for (const auto& point : data) {
      auto id = std::get<0>(point);
      auto x = std::get<1>(point);
      auto y = std::get<2>(point);

      std::cout << "ID: " << id << ", X: " << x << ", Y: " << y << std::endl;
  }
  std::cout << "------\n";
}

void GraphManager::DrawAreaGraph(const std::vector<std::tuple<int, double, int, float>>& data, const QString& graphTitle) {
  if (data.empty()) {
      qDebug() << "No data to display.";
      return;
  }

  std::vector<std::tuple<int, double, int, float>> sortedData = data;
  SortById(sortedData);

  std::vector<std::tuple<int, double, int, float>> sortedByDateData = data;
  SortByDate(sortedByDateData);

  QChart* chart = new QChart();
  chart->setTitle(graphTitle);

         // Initialize min and max values for chart axis
  qreal minX = 0;
  qreal maxX = std::get<1>(sortedByDateData.back());
  qreal minY = 0;  // Start y-axis from 0
  qreal maxY = 0;  // Will be calculated based on stacked data

         // Set to store unique species IDs

  std::map<double, int> speciesAtTime;
  std::set<int> uniqueIds;
  for (const auto& entry : sortedData) {
      double time = std::get<1>(entry);
      // Check if this time point is already in the map
      if (speciesAtTime.find(time) == speciesAtTime.end()) {
          // Not found, initialize the count for this time
          speciesAtTime[time] = 1;
      } else {
          // Found, increment the count for this time
          speciesAtTime[time]++;
      }
      uniqueIds.insert(std::get<0>(entry));
  }

         // Map to keep track of the cumulative height at each x for stacking
  std::map<double, double> cumulativeHeightAtX;

  for (int id : uniqueIds) {
      QLineSeries* upperSeries = new QLineSeries();
      QLineSeries* lowerSeries = new QLineSeries();
      bool first_time = true;
      float color = 0;
      for (const auto& entry : sortedData) {
        if (std::get<0>(entry) == id) {
          if(first_time) {
            color = std::get<3>(entry);
            first_time = false;
          }
          double x = std::get<1>(entry);
          double y = std::get<2>(entry);

          double lowerBound = cumulativeHeightAtX[x];
          double upperBound = lowerBound + y;

                 // Append data points to series
          upperSeries->append(x, upperBound);
          lowerSeries->append(x, lowerBound);

                 // Update the cumulative height for stacking
          cumulativeHeightAtX[x] = upperBound;

                 // Update maxY
          maxY = std::max(maxY, 4/3 * upperBound);

        }
      }

      QAreaSeries* areaSeries = new QAreaSeries(upperSeries, lowerSeries);
      areaSeries->setName(QString("Species %1").arg(id));
      areaSeries->setProperty("speciesId", QVariant(id));

      qreal hue = color;
      areaSeries->setColor(QColor::fromHsvF(hue, 0.75, 0.75));
      areaSeries->setBorderColor(QColor::fromHsvF(hue, 0.75, 0.75).darker());
      connect(areaSeries, &QAreaSeries::hovered, [this, areaSeries](const QPointF &point, bool state) {
          this->onAreaHovered(areaSeries, point, state);
      });

             // Add series to chart
      chart->addSeries(areaSeries);
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

void GraphManager::onAreaHovered(QAreaSeries *series, QPointF point, bool state) {
    if (state) {
        // Retrieve the custom property
        int speciesId = series->property("speciesId").toInt();

        // Create the tooltip text
        QString tooltipText = QString("Species ID: %1")
                              .arg(speciesId);

        // Show the tooltip
        QToolTip::showText(QCursor::pos(), tooltipText);

        simulationCanvas_->GetInfoPanel().SetSelectedSpecies(speciesId);
    } else {
        // Hide the tooltip when not hovering
        QToolTip::hideText();
        simulationCanvas_->GetInfoPanel().RemoveSelectedSpecies();

    }
}

void GraphManager::DrawSpeciesArea() {
  auto data = cluster_->getSpeciesData();
  DrawAreaGraph(data, "Species Population over Time");
}

void GraphManager::DrawSizeEnergyScatterplot() {
  auto& infoPanel = simulationCanvas_->GetInfoPanel();
  auto selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and energy data
  std::vector<double> sizes;
  std::vector<double> energies;

  for (const auto& creature : data->creatures_) {
      float creatureSize = creature->GetSize();
      float creatureEnergy = creature->GetEnergy();

      // Check if the creature is selected
      if (selectedCreature && creature->GetID() == selectedCreature->GetID()) {
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
  auto selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and velocity data
  std::vector<double> sizes;
  std::vector<double> velocities;

  for (const auto& creature : data->creatures_) {
      float creatureSize = creature->GetSize();
      float creatureVelocity = creature->GetVelocity();

      // Check if the creature is selected
      if (selectedCreature && creature->GetID() == selectedCreature->GetID()) {
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
  auto selectedCreature = infoPanel.GetSelectedCreature();

  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and velocity data
  std::vector<double> energies;
  std::vector<double> velocities;

  for (const auto& creature : data->creatures_) {
      float creatureEnergy = creature->GetEnergy();
      float creatureVelocity = creature->GetVelocity();

      // Check if the creature is selected
      if (selectedCreature && creature->GetID() == selectedCreature->GetID()) {
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
    qDebug() << "Calling DrawCreaturesSizeOverTimeGraph";
    DrawCreaturesSizeOverTimeGraph();
  }
  if (index == 3) {
    qDebug() << "Calling DrawCreaturesEneergyOverTimeGraph";
    DrawCreaturesEnergyOverTimeGraph();
  }
  if (index == 4) {
    qDebug() << "Calling DrawCreaturesVelocityOverTimeGraph";
    DrawCreaturesVelocityOverTimeGraph();
  }
  if (index == 5) {
    qDebug() << "Calling DrawCreaturesDietOverTimeGraph";
    DrawCreaturesDietOverTimeGraph();
  }
  if (index == 6) {
    qDebug() << "Calling DrawCreaturesOffspringOverTimeGraph";
    DrawCreaturesOffspringOverTimeGraph();
  }
  if (index == 7) {
    qDebug() << "Calling DrawCreaturesScatterplot";
    DrawSizeEnergyScatterplot();
  }
  if (index == 8) {
    qDebug() << "Calling DrawCreaturesScatterplot";
    DrawSizeVelocityScatterplot();
  }
  if (index == 9) {
    qDebug() << "Calling DrawCreaturesScatterploth";
    DrawEnergyVelocityScatterplot();
  }

  if (index == 10) {
    qDebug() << "Calling DrawSpeciesArea";
    DrawSpeciesArea();
  }

}
