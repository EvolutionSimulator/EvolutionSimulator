#include "mainwindow/graph_manager.h"

#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>
#include <QValueAxis>

GraphManager::GraphManager(QWidget* parent, Engine *engine, SimulationCanvas* simulationCanvas) :
    QObject(nullptr),
    parent_(parent),
    engine_(engine),
    simulationCanvas_(simulationCanvas){}

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

void GraphManager::DrawSpeciesArea() {
  // Sample data vector for testing
  std::vector<std::vector<std::pair<double, double>>> testData = {
      {{0.0, 0.0}, {0.0, 1.0}, {0.0, 2.0}, {0.0, 3.0}, {0.0, 4.0}},   // Series with ID 0
      {{1.0, 1.0}, {1.0, 2.0}, {1.0, 3.0}, {1.0, 4.0}, {1.0, 5.0}},   // Series with ID 1
      {{2.0, 2.0}, {2.0, 3.0}, {2.0, 4.0}, {2.0, 3.0}, {2.0, 2.0}}    // Series with ID 2
      // Add more series as needed
  };
  // Check if testData is not empty
  if (!testData.empty()) {
      // Ensure each series is not empty
      for (const auto& series : testData) {
          if (series.empty()) {
              qDebug() << "Error: Series is empty.";
              return;
          }
      }
      qDebug() << "testData is not empty.";
      // Call DrawAreaGraph with the valid testData
      DrawAreaGraph(testData, "Test Area Graph");
  } else {
      qDebug() << "Error: testData is empty.";
  }
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
