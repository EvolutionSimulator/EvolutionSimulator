# Week 1 - created main structure of the enitities classes
- added a general entity class, and a food class and a creature class inheriting from it
- added an environment class (holding bariables like map size, food density, etc.)
- added entity vectors and environment in simulation data
- added random initialization and linked it with the simulation
- started the implementation of spatial partitioning and checking collisions

# Week 2 - working on collisions
- Berra completed spatial partitioning and checking collisions and I helped integrate it
- refactored some of the classes to work with pointers
- added state variable to entities, and deletion of dead entities in the simulation loop
- added gtests for collisions
- started the implementation of a torus map

# Week 3 - fixing bugs and integrating three projects
- worked with Carlos on bringing everything together in the system-links/main to see the first form of evolution
- added a lot of google test, debugging various parts of the project, and helping other people (Milan, Berra, Angela, Adrien) find bugs in their implementations
- added MutateChangeBias mutation in NEAT and fixed food initialization

# Week 4 - implementing vision
- implemented a vision system for creatures with: vision radius and angle centered at their orientation
- changed thinking logic so that creatures only detect food in line of sight, which Timofey optimized with a BFS search
- added tests, fixed bugs and added unique creature IDs and vision cones for the UI

# Week 5 - implementing digestion
- improved handling of edge cases for vision 
- implemented draft of click and drag for moving the map in UI (which Carlos smoothened)
- implemented digestion: stomach capacity, fullness, acid and diet (herbivores and carnivores multipliers)
- implemented biting logic: bite strength, eating cooldown, etc.
- integrated digestion into the neural network, mutables and collisions, added tests

# Week 6
- Added stomach with shaders in the UI
- Coordinated attack project, assigning tasks and double checking people’s implementations in Bite and Grab
- Found the bug and made restart crash (in the way we define the minimal viable genomes) and tried to implement an artificial predator genome
- Helped with the refactor of the Creature class
- Refactored MainWindow and fixed configuration screen




