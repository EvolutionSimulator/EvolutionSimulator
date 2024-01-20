#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPainter>
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
#include "QtWidgets/qpushbutton.h"
#include "engine.h"
#include "simulationcanvas/simulationcanvas.h"

class GraphManager : public QObject {
  Q_OBJECT  // Enable signal and slot mechanism

public:
  explicit GraphManager(QWidget* parent, Engine* engine, SimulationCanvas* simulationCanvas);
  void SetEngine(Engine* engine);

  // Helper function to create a directory if it doesn't exist
  bool createDirectory(const QString& path) {
      QDir dir(path);
      if (!dir.exists()) {
          return dir.mkpath(path);
      }
      return true;
  }

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

    // Add a Save button to the dialog
    QPushButton* saveButton = new QPushButton("Save Graph");
    layout->addWidget(saveButton);

    // Connect the Save button to a slot that saves the graph
    connect(saveButton, &QPushButton::clicked, [chartView, data, graphTitle, this]() {
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString folderPath = desktopPath + "/EvolutionSimulationData/";

        // Create the folder if it doesn't exist
        if (createDirectory(folderPath)) {
            QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Graph", folderPath, "PNG Image (*.png);;CSV File (*.csv)");
            if (!filePath.isEmpty()) {
                // Save image
                QString imageFilePath = filePath + ".png";
                chartView->grab().save(imageFilePath);

                // Save data to CSV file
                QString csvFilePath = filePath + ".csv";
                QFile file(csvFilePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream stream(&file);
                    for (size_t i = 0; i < data.size(); ++i) {
                        stream << i << "," << data[i] << "\n";
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

  //helper function to take the absolute values of each entry in a vector
  std::vector<double> absoluteValues2(const std::vector<double>& input) {
    std::vector<double> result(input.size());

    std::transform(input.begin(), input.end(), result.begin(), [](double value) {
        return std::abs(value);
    });

    return result;
  }

  // Function to draw scatter plot
  void DrawScatterPlot(const std::vector<double>& data1, const std::vector<double>& data2,
                       const QString& title, const QString& xAxisLabel, const QString& yAxisLabel) {
    if (data1.size() != data2.size()) {
      qDebug() << "Error: Input data lengths do not match.";
      return;
    }

    QScatterSeries* series_red = new QScatterSeries();
    QScatterSeries* series_blue = new QScatterSeries();

    series_red->setBrush(QBrush(Qt::red));
    series_blue->setBrush(QBrush(Qt::blue));

    for (size_t i = 0; i < data1.size(); ++i) {
      double x = data1[i];
      double y = data2[i];

      // Check if the value is negative
      if (x < 0 || y < 0) {
          x = x*-1;
          y = y*-1;
          // Display negative values in red
          QPointF point(x, y);
          series_red->append(point);
      } else {
          // Display positive values in their original color
          series_blue->append(x, y);
      }
    }

    QChart* chart = new QChart();
    chart->addSeries(series_blue);
    chart->addSeries(series_red);

    // Set custom axis ranges with margins
    double minX = *std::min_element(data1.begin(), data1.end()) * 0.95;
    double maxX = std::max(*std::max_element(data1.begin(), data1.end()), -(*std::min_element(data1.begin(), data1.end()))) * 1.1;
    double minY = *std::min_element(data2.begin(), data2.end()) * 0.95;
    double maxY = std::max(*std::max_element(data2.begin(), data2.end()), -(*std::min_element(data2.begin(), data2.end()))) * 1.1;

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText(xAxisLabel);
    chart->axes(Qt::Vertical).first()->setTitleText(yAxisLabel);
    chart->axes(Qt::Horizontal).first()->setRange(minX, maxX);
    chart->axes(Qt::Vertical).first()->setRange(minY, maxY);

    chart->legend()->markers(series_red)[0]->setVisible(false);
    chart->legend()->markers(series_blue)[0]->setVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog dialog;
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(chartView);
    dialog.setLayout(layout);
    dialog.resize(800, 600);
    dialog.setWindowTitle(title);

    // Add a Save button to the dialog
    QPushButton* saveButton = new QPushButton("Save Scatter Plot");
    layout->addWidget(saveButton);

    // Connect the Save button to a slot that saves the scatter plot
    connect(saveButton, &QPushButton::clicked, [chartView, data1, data2, title, this]() {
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString folderPath = desktopPath + "/EvolutionSimulationData/";

        // Create the folder if it doesn't exist
        if (createDirectory(folderPath)) {
            QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Scatter Plot", folderPath, "PNG Image (*.png);;CSV File (*.csv)");
            if (!filePath.isEmpty()) {
                // Save image
                QString imageFilePath = filePath + ".png";
                chartView->grab().save(imageFilePath);

                // Save data to CSV file
                QString csvFilePath = filePath + ".csv";
                QFile file(csvFilePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream stream(&file);
                    for (size_t i = 0; i < data1.size(); ++i) {
                        stream << data1[i] << "," << data2[i] << "\n";
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
    emit resetGraphMenuIndex();

    dialog.exec();
  }

public slots:
  void DrawCreaturesOverTimeGraph();
  void DrawCreaturesSizeOverTimeGraph();
  void DrawCreaturesEnergyOverTimeGraph();
  void DrawCreaturesVelocityOverTimeGraph();
  void DrawSizeEnergyScatterplot();
  void DrawSizeVelocityScatterplot();
  void handleDropdownSelection(int index);

private:
  QWidget* parent_;
  Engine* engine_;
  SimulationCanvas* simulationCanvas_;
  InfoPanel* GetInfoPanel();

signals:
  void resetGraphMenuIndex();
};



#endif // GRAPH_MANAGER_H
