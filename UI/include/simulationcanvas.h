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

    void DrawGraph(sf::RenderWindow& window, double (*graphFunction)(double), double xMin, double xMax, double yMin, double yMax);

    // New function declaration for drawing the number of creatures over time
    void DrawCreatureCountOverTime(sf::RenderWindow& window, const std::vector<Creature>& creatures);

private :
    virtual void OnInit() override;
    virtual void OnUpdate() override;

    void RenderSimulation(SimulationData* data);

    Simulation* simulation_ = nullptr;
    std::function<void(SimulationData*)> render_lambda_; // we will pass this to the ProcessData method of Simulation
};
