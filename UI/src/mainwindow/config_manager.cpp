#include "mainwindow/config_manager.h"
#include "qicon.h"
#include "qlabel.h"
#include "qslider.h"
#include <QDialog>
#include <QVBoxLayout>

ConfigManager::ConfigManager(QWidget* parent, Engine *engine) :
                                                              QObject(nullptr),
                                                              parent_(parent),
                                                              engine_(engine){
}

void ConfigManager::ChangeFoodDensity(int value) {
  food_density_ = static_cast<double>(value) / 1000.0;     // Convert to density
  engine_->GetEnvironment().SetFoodDensity(food_density_); // Update the density
  engine_->UpdateEnvironment(); // Apply the updated density
  emit UpdateUIForConfigScreen(food_density_, friction_coefficient_);
}

void ConfigManager::SetEngine(Engine* engine)
{
  engine_ = engine;
}

void ConfigManager::ChangeEngineSpeed(double value) {
  engine_->SetSpeed(value/100.0);
}

void ConfigManager::ChangeFriction(double value) {
  friction_coefficient_ = static_cast<double>(value) / 100.0;  // Scale slider value to be in the range 0.05 to 0.20
  engine_->GetEnvironment().SetFrictionalCoefficient(friction_coefficient_);
  engine_->UpdateEnvironment();
  emit UpdateUIForConfigScreen(food_density_, friction_coefficient_);
}

void ConfigManager::ShowConfigScreen(){

  emit PauseSimulation();
  double initial_speed = engine_->GetSpeed();
  double frictional_coefficient = engine_->GetEnvironment().GetFrictionalCoefficient();

         // Create a new QDialog (config dialog)
  QDialog* configDialog = new QDialog(parent_);
  configDialog->setWindowTitle("Configuration");

         // Get the size of the main window
  QSize mainWindowSize = parent_->size();

         // Set the size of the configuration dialog based on the main window size
  int dialogWidth = mainWindowSize.width() / 3;
  int dialogHeight = mainWindowSize.height() / 3;
  configDialog->resize(dialogWidth, dialogHeight);


         //Create layouts
  QVBoxLayout* mainLayout = new QVBoxLayout(configDialog);
  QVBoxLayout* titleLayout = new QVBoxLayout();
  QVBoxLayout* contentLayout = new QVBoxLayout();

  mainLayout->addLayout(titleLayout);
  mainLayout->addLayout(contentLayout);

  QLabel* titleLabel = new QLabel("<html><h1><b>Configuration Options</b></h1></html>", configDialog);
  QLabel* foodDLabel = new QLabel("Food density:", configDialog);

  QSlider* speedSlider = new QSlider(Qt::Horizontal, configDialog);
  speedSlider-> setMinimum(1);
  speedSlider-> setMaximum(1000);
  speedSlider-> setSingleStep(1);
  speedSlider->setValue(initial_speed * 100);

  QSlider* foodDSlider = new QSlider(Qt::Horizontal, configDialog);
  QSlider* frictionSlider = new QSlider(Qt::Horizontal, configDialog);
  frictionSlider-> setMinimum(5);
  frictionSlider-> setMaximum(100);
  frictionSlider-> setSingleStep(1);
  frictionSlider->setValue(static_cast<int>(frictional_coefficient * 100));


  titleLayout->addWidget(titleLabel, Qt::AlignTop | Qt::AlignHCenter);
  QLabel* speedValueLabel = new QLabel(QString("Simulation Speed: %1").arg(initial_speed));
  contentLayout->addWidget(speedValueLabel);
  contentLayout->addWidget(speedSlider);

  contentLayout->addWidget(foodDLabel);
  contentLayout->addWidget(foodDSlider);

  QLabel* frictionValueLabel = new QLabel(QString("Frictional Coefficient: %1").arg(frictional_coefficient));
  contentLayout->addWidget(frictionValueLabel);
  contentLayout->addWidget(frictionSlider);


  titleLayout->setContentsMargins(20,5,20,20);
  contentLayout->setContentsMargins(20,5,20,5);
  mainLayout->setContentsMargins(5,5,5,5);

  connect(speedSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeEngineSpeed);
  connect(foodDSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeFoodDensity);
  connect(frictionSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeFriction);

  // Create a local pointer to configDialog
  QDialog* localConfigDialog = configDialog;

  // Connect the valueChanged signal of the speedSlider to update the speedValueLabel
  connect(speedSlider, &QSlider::valueChanged, [speedValueLabel, localConfigDialog](int value) {
      double speedValue = static_cast<double>(value) / 100.0;  // Convert to actual speed value
      speedValueLabel->setText(QString("Simulation Speed: %1").arg(speedValue));

      // Now, you can use localConfigDialog here if needed
  });

  // Connect the valueChanged signal of the frictionSlider to update the frictionValueLabel
  connect(frictionSlider, &QSlider::valueChanged, [frictionValueLabel, localConfigDialog](int value) {
      double frictionValue = static_cast<double>(value) / 100.0;  // Convert to actual friction value
      frictionValueLabel->setText(QString("Frictional Coefficient: %1").arg(frictionValue));

      // Now, you can use localConfigDialog here if needed
  });

         // Show the configuration dialog modally
  configDialog->setLayout(mainLayout);
  configDialog->exec();

  emit ResumeSimulation();
}


void ConfigManager::ChangeCreatureDensity(int value) {
    creature_density_ = static_cast<double>(value) / 10000.0; // Convert to density
    emit RestartSimulationRequested(food_density_, creature_density_);
}
