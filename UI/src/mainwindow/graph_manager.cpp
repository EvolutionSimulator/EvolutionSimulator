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




void GraphManager::DrawGraph(std::vector<double> data, const QString& graphTitle) {

  if (data.empty()) {
    qDebug() << "No data to display.";
    return;
  }

  QLineSeries* series = new QLineSeries();
  for (size_t i = 0; i < data.size(); ++i) {
    series->append(i, data[i]);
  }

  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->createDefaultAxes();
  chart->axes(Qt::Horizontal).first()->setTitleText("Time Elapsed");
  chart->axes(Qt::Vertical).first()->setTitleText(graphTitle);

  QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
  if (axisY) {
    axisY->setRange(0.0, 4.0/3.0 * axisY->max()); // Set the minimum to 0.0 and keep the current maximum
  }

  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  QDialog* dialog = new QDialog(parent_);
  dialog->setWindowTitle(graphTitle);
  QVBoxLayout* layout = new QVBoxLayout(dialog);
  layout->addWidget(chartView);
  dialog->setLayout(layout);
  dialog->resize(800, 600);

  connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
  dialog->exec();
}

void GraphManager::DrawCreaturesOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureCountOverTime();
  DrawGraph(data, "Number of Creatures in the Simulation");
}

void GraphManager::DrawCreaturesSizeOverTimeGraph() {
  auto data = engine_->GetSimulation()->GetSimulationData()->GetCreatureSizeOverTime();
  DrawGraph(data, "Average Size of Creatures in the Simulation");
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
}
