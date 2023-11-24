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
    virtual void OnInit() override;
    virtual void OnUpdate() override;

    void RenderSimulation(SimulationData* data);

    Simulation* simulation_ = nullptr;
    std::function<void(SimulationData*)> render_lambda_;
};
