#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QObject>
#include <QWidget>
#include "QtWidgets/qlabel.h"
#include "core/engine.h"

class ConfigManager : public QObject {
  Q_OBJECT  // Enable signal and slot mechanism

public:
  explicit ConfigManager(QWidget* parent, Engine* engine);
  void SetEngine(Engine* engine);

public slots:
  void ChangeFoodDensity(int value);
  void ChangeFriction(double value);
  void ChangeEngineSpeed(double value);
  void ChangeCreatureDensity(int value);
  void ShowConfigScreen();

signals:
  void RestartSimulationRequested(double food_density, double creature_density);
  void UpdateUIForConfigScreen(double food_density, double friction_coefficient);
  void PauseSimulation();
  void ResumeSimulation();

private:
  double creature_density_ = 0.001;
  double friction_coefficient_ = 0.05;
  double food_density_ = 0.001;
  QWidget* parent_;
  Engine* engine_;
  QLabel* speedValueLabel;
};


#endif // CONFIG_MANAGER_H
