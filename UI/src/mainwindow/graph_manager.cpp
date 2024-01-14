#include "mainwindow/graph_manager.h"

#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>

GraphManager::GraphManager(QWidget* parent, Engine *engine) :
                                                              QObject(nullptr),
                                                              parent_(parent),
                                                              engine_(engine){}



void GraphManager::DrawCreaturesOverTimeGraph() {
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
    QDialog* dialog = new QDialog(parent_);

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

void GraphManager::DrawCreaturesSizeOverTimeGraph() {
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
    QDialog* dialog = new QDialog(parent_);
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

void GraphManager::handleDropdownSelection(int index) {
  // Here, you can call the function corresponding to the selected item
  qDebug() << "Dropdown selection changed to index:" << index;

  if (index == 1) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesOverTimeGraph();
  }
  if (index == 2) {
    qDebug() << "Calling DrawCreaturesOverTimeGraph";
    DrawCreaturesSizeOverTimeGraph();
  }
}
