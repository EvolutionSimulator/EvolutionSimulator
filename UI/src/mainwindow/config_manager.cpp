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
  friction_coefficient_ = 0.0;
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

void ConfigManager::ChangeEngineSpeed(int value) {
  engine_->SetSpeed(value);
}

void ConfigManager::ChangeFriction(int value) {
  friction_coefficient_ = static_cast<double>(value) / 100.0;  // Scale slider value to be in the range 0.05 to 0.20
  engine_->GetEnvironment().SetFrictionalCoefficient(friction_coefficient_);
  engine_->UpdateEnvironment();
  emit UpdateUIForConfigScreen(food_density_, friction_coefficient_);
}

void ConfigManager::ShowConfigScreen(){

  emit PauseSimulation();

  double initial_creature_density = engine_->GetEnvironment().GetCreatureDensity();

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
  QLabel* speedLabel = new QLabel("Simulation Speed:", configDialog);
  QLabel* foodDLabel = new QLabel("Food density:", configDialog);
  QLabel* frictionLabel = new QLabel("Friction:", configDialog);

  QSlider* speedSlider = new QSlider(Qt::Horizontal, configDialog);
  speedSlider-> setMinimum(0);
  speedSlider-> setMaximum(5);
  speedSlider-> setSingleStep(1);
  speedSlider->setTickPosition(QSlider::TicksBelow);

  QSlider* foodDSlider = new QSlider(Qt::Horizontal, configDialog);
  QSlider* frictionSlider = new QSlider(Qt::Horizontal, configDialog);

  titleLayout->addWidget(titleLabel, Qt::AlignTop | Qt::AlignHCenter);
  contentLayout->addWidget(speedLabel);
  contentLayout->addWidget(speedSlider);

  contentLayout->addWidget(foodDLabel);
  contentLayout->addWidget(foodDSlider);

  contentLayout->addWidget(frictionLabel);
  contentLayout->addWidget(frictionSlider);


  titleLayout->setContentsMargins(20,5,20,20);
  contentLayout->setContentsMargins(20,5,20,5);
  mainLayout->setContentsMargins(5,5,5,5);

  connect(speedSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeEngineSpeed);
  connect(foodDSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeFoodDensity);
  connect(frictionSlider, &QSlider::valueChanged, this, &ConfigManager::ChangeFriction);

         // Show the configuration dialog modally
  configDialog->setLayout(mainLayout);
  configDialog->exec();

  emit ResumeSimulation();
}


void ConfigManager::ChangeCreatureDensity(int value) {
    creature_density_ = static_cast<double>(value) / 10000.0; // Convert to density
    emit RestartSimulationRequested(food_density_, creature_density_);
}
