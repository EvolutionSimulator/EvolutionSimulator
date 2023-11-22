#pragma once

#include <engine.h>
#include <SFML/Graphics.hpp>
#include "qsfmlcanvas.h"
#include "simulation.h"

class SimulationCanvas : public QSFMLCanvas
{
    Q_OBJECT

public :
    SimulationCanvas(QWidget* Parent);

    void SetSimulation(Simulation* simulation);
    Simulation* GetSimulation();

private :
    void OnInit();
    void OnUpdate();

    Simulation* simulation_ = nullptr;
};
