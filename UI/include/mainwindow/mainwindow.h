#pragma once

#include <QMainWindow>
#include <thread>

#include "QtCharts/qlineseries.h"
#include "QtWidgets/qslider.h"
#include "engine.h"
#include "graph_manager.h"
#include "config_manager.h"

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

  void SetUpConnections();
  void DrawUI();

  void InitializeEngine();
  void InitializeEngineWithDensities(double food_density, double creature_density);

  void keyPressEvent(QKeyEvent *event) override; // Add this line

 private slots:  // Slots should be placed under this section
  void RunSimulation();
  void PauseSimulation();
  void ResumeSimulation();
  void KillEngine();
  void ToggleSimulation();
  void RestartSimulation();
  void handleUIUpdateForConfigScreen(double food_density, double friction_coefficient);
  void handleRestartSimulationRequested(double food_density, double creature_density);

  private:

  Ui::MainWindow* ui_;  // Pointer to all UI widgets
  Engine* engine_ = nullptr;  // Pointer to the simulation engine
  std::thread engine_thread_;  // Thread for running the simulation engine

  double lastRecordedTime_;
  QTimer *updateTimer;

  GraphManager* graph_manager_;
  ConfigManager* config_manager_;
};
