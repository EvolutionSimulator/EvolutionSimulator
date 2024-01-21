#include "mainwindow/graph_manager.h"

#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>
#include <QValueAxis>

GraphManager::GraphManager(QWidget* parent, Engine *engine) :
                                                              QObject(nullptr),
                                                              parent_(parent),
                                                              engine_(engine){}

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

void GraphManager::DrawSizeEnergyScatterplot() {
  auto data = engine_->GetSimulation()->GetSimulationData();

  // Vectors to store size and energy data
  std::vector<double> sizes;
  std::vector<double> energies;

  for (const auto& creature : data->creatures_) {
      float creatureSize = creature->GetSize();
      float creatureEnergy = creature->GetEnergy();

      // Append size and energy data to vectors
      sizes.push_back(creatureSize);
      energies.push_back(creatureEnergy);
  }

  DrawScatterPlot(sizes, energies, "Scatterplot of Creature Size and Energy",
                  "Creature Size", "Creature Energy");
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

}
