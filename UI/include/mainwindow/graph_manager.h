#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QDialog>
#include <QVBoxLayout>
#include "engine.h"

class GraphManager : public QObject {
  Q_OBJECT  // Enable signal and slot mechanism

public:
  explicit GraphManager(QWidget* parent, Engine* engine);
  void SetEngine(Engine* engine);

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

    emit resetGraphMenuIndex();

    dialog->exec();
  }

  // Function to draw scatter plot
  void DrawScatterPlot(const std::vector<double>& data1, const std::vector<double>& data2,
                       const QString& title, const QString& xAxisLabel, const QString& yAxisLabel) {
    if (data1.size() != data2.size()) {
      qDebug() << "Error: Input data lengths do not match.";
      return;
    }

    QScatterSeries* series = new QScatterSeries();

    for (size_t i = 0; i < data1.size(); ++i) {
      series->append(data1[i], data2[i]);
    }

    QChart* chart = new QChart();
    chart->addSeries(series);

    // Set custom axis ranges with margins
    double minX = *std::min_element(data1.begin(), data1.end()) * 0.95;
    double maxX = *std::max_element(data1.begin(), data1.end()) * 1.1;
    double minY = *std::min_element(data2.begin(), data2.end()) * 0.8;
    double maxY = *std::max_element(data2.begin(), data2.end()) * 1.1;

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText(xAxisLabel);
    chart->axes(Qt::Vertical).first()->setTitleText(yAxisLabel);
    chart->axes(Qt::Horizontal).first()->setRange(minX, maxX);
    chart->axes(Qt::Vertical).first()->setRange(minY, maxY);

    chart->legend()->markers(series)[0]->setVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog dialog;
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(chartView);
    dialog.setLayout(layout);
    dialog.resize(800, 600);
    dialog.setWindowTitle(title);

    emit resetGraphMenuIndex();

    dialog.exec();
  }

public slots:
  void DrawCreaturesOverTimeGraph();
  void DrawCreaturesSizeOverTimeGraph();
  void DrawCreaturesEnergyOverTimeGraph();
  void DrawCreaturesVelocityOverTimeGraph();
  void DrawSizeEnergyScatterplot();
  void handleDropdownSelection(int index);

private:
  QWidget* parent_;
  Engine* engine_;

signals:
  void resetGraphMenuIndex();
};



#endif // GRAPH_MANAGER_H
