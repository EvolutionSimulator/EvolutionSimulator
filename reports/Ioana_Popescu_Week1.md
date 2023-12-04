# EvolutionSimulator

On the branch entity-creation:
- added a general entity class, which has as attributes: x_coordinate, y_coordinate and size
- a food class and a creature class inheriting from the entity class, which have extra attributes
- an environment class that just holds some constants about map size and density for initialization 
 
All of these are linked to the simulation data class, where I added a creature vector, a food vector and an environment, that will be the data stractures that help us update the simulation at every step, and which will be sent to the UI part.
 
To test my implementation, I randomly initialize some creatures and some food (functions called in the constructor of SimulationData class) and display it. I apply a random test function (ModifyAllCreatures in simulationdata.cpp) to test everything is linked properly (SimulationData, Simulation and UI).

On the branch collisions:
- Researched two efficient methods for checking collisions: sweep and prune and spatial partioning
- Implemented spatial partitioning, with grids of cell size given in the environment class
- Implemented a function that updates the grid and checks for collisions at each FixedUpdate (making sure frame rate doesn't drop)

