#pragma once

#include <QMainWindow>
#include <thread>

#include "QtCharts/qlineseries.h"
#include "QtWidgets/qslider.h"
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

  void InitializeEngine();

  void keyPressEvent(QKeyEvent *event) override; // Add this line

 private slots:  // Slots should be placed under this section
  void RunSimulation();
  void PauseSimulation();
  void ResumeSimulation();
  void KillEngine();
  void ToggleSimulation();
  void RestartSimulation();
  void ChangeFoodDensity(
      int value);  // Make sure this is under 'private slots:'
  void ChangeCreatureDensity(int value);
  void ChangeFriction(int value);
  void DrawCreaturesOverTimeGraph();
  void DrawCreaturesSizeOverTimeGraph();
  void handleDropdownSelection(int index);
  void ShowConfigScreen();

  private:
  double friction_coefficient;
  double creature_density = 0.001;
  double food_density = 0.001;
  Ui::MainWindow* ui_;  // Pointer to all UI widgets

  Engine* engine_ = nullptr;  // Pointer to the simulation engine

  std::thread engine_thread_;  // Thread for running the simulation engine

  double lastRecordedTime_;
  QTimer *updateTimer;
  QSlider* slider;

};
