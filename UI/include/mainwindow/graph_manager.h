#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>
#include "engine.h"

class GraphManager : public QObject {
  Q_OBJECT  // Enable signal and slot mechanism

public:
  explicit GraphManager(QWidget* parent, Engine* engine);
  template <typename T> void DrawGraph(std::vector<T> data, const QString& graphTitle) {

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
    chart->axes(Qt::Vertical).first()->setRange(0, *std::max_element(data.begin(), data.end()) * 1.2);

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

public slots:
  void DrawCreaturesOverTimeGraph();
  void DrawCreaturesSizeOverTimeGraph();
  void DrawCreaturesEnergyOverTimeGraph();
  void DrawCreaturesVelocityOverTimeGraph();
  void handleDropdownSelection(int index);

private:
  QWidget* parent_;
  Engine* engine_;
};



#endif // GRAPH_MANAGER_H
