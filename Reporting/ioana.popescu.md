# Week 1 - structuring the simulation loop and entity classes
- Added a general entity class, and a food class and a creature class inheriting from it
- Added an environment class (holding bariables like map size, food density, etc.)
- Added entity vectors and environment in simulation data
- Added random initialization and linked it with the simulation
- Started the implementation of spatial partitioning and checking collisions

# Week 2 - working on collisions
- Berra completed spatial partitioning and checking collisions and I helped integrate it, as her branch was crashing
- Refactored some of the classes to work with pointers
- Added state variable to entities, and deletion of dead entities in the simulation loop
- Added google tests for collisions, detecting bugs
- Started the implementation of a torus map

# Week 3 - fixing bugs and integrating three projects
- Worked with Carlos on bringing everything together in the system-links/main to see the first form of evolution
- Added a lot of google test, debugging various parts of the project, and helping other people (Milan, Berra, Angela, Adrien) find bugs in their implementations
- Added MutateChangeBias mutation in NEAT and fixed food initialization

# Week 4 - implementing vision
- Implemented a vision system for creatures with: vision radius and angle centered at their orientation
- Changed thinking logic so that creatures only detect food in line of sight, which Timofey optimized with a BFS search
- Added tests, fixed bugs, improving handling of edge cases for vision 
- Added vision cones in the UI

# Week 5 - implementing digestion 
- Implemented click and drag for moving the map in UI (which Carlos smoothened)
- Implemented digestion: stomach capacity, fullness, acid and diet (herbivores and carnivores multipliers)
- Implemented biting logic: bite strength, eating cooldown, etc.
- Integrated digestion into the neural network, mutables and collisions, added tests

# Week 6 - coordinating attack project and refactoring
- Added stomach with shaders in the UI
- Coordinated attack project, assigning tasks and double checking people’s implementations in Bite and Grab
- Found the bug and made restart crash (in the way we define the minimal viable genomes) and tried to implement an artificial predator genome
- Helped with the refactor of the Creature class
- Refactored MainWindow (dividing it into a class for the configuration screen, and one for graphs), fixed the link between the configuration screen and the engine, made template function for graphs

  # Week 7
- Refactored SimulationCanvas (dividing it into an Info Panel manager and a Texture manager), removing a lot of redundant code, improving code readability and functionality
- Made info panel compatible with zooming and dissapear on restart
- Made selecting creatures compatible with warping around the torus map
- Re-vamped vision system to detect color and compatibility for more open evolution, and made it work around the torus map
  




