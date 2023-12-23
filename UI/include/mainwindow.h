#pragma once

#include <QMainWindow>
#include <thread>

#include "QtCharts/qlineseries.h"
#include "engine.h"

QT_BEGIN_NAMESPACE  //
    namespace Ui {
  class MainWindow;
}  // <- generated by Qt, don't touch
QT_END_NAMESPACE  //

    class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void SetEngine(Engine* engine);

 private slots:  // Slots should be placed under this section
  void RunSimulation();
  void ChangeFoodDensity(
      int value);  // Make sure this is under 'private slots:'
  void ChangeCreatureDensity(int value);
  void ToggleSimulation();
  void RestartSimulation();
  void DisplayGraph();
  void GraphExampleFunction();
  void ChangeFriction(int value);
  void DrawCreaturesOverTimeGraph();

 private:
  double friction_coefficient;
  double creature_density = 0.001;
  double food_density = 0.001;
  Ui::MainWindow* ui_;  // Pointer to all UI widgets

  Engine* engine_;  // Pointer to the simulation engine

  std::thread engine_thread_;  // Thread for running the simulation engine

  double lastRecordedTime_;
  QTimer *updateTimer;

};
