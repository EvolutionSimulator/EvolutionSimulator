# Week 1 - Setting up
- Organized Notion infrastructure
- Defined the road map for the project
- Project leader for the "entity creation" project
- Created a Food Generation function
- Helped define some of the class structure of the project

# Week 2 - Linking everything
- Project leader for the "system links" project
- Created a reproduction function with all of the corresponding adjustments 
- Implemented reproduction into the game loop using a queue

# Week 3 - Evolution!
- Took care of organizing and merging the "system-links" project collaborating with Ioana
- Fixed multiple problems with pointer management that ended up causing segmentation faults
- Researched how to implement procedural sprite generation
- Build the brain-creature interface
- Achieved the first instance of evolution in our project
- Documented the entire codebase so that it's compatible with Doxygen

# Week 4 - Cleaner Implementation
- Fixed different bugs with the UI and Windows interfaces that use the screen ampliation recommended settings
- Fixed two bugs that caused the new movement system to allow creatures to reach infinite speeds by exploiting errors in the system
- Researched how to implement species categorization with clustering algorithms
- Implemented mutable project allowing size, energy storage, and other characteristics to be subject to natural selection  

# Week 5 - UI Revamp
- Research how to deal with files during compilation and figure out how to access files using the QResources handler
- Worked on a complete revamp of the UI looks and created different sprites to substitute the previous buttons
- Created sprites with procedural generation depending on size, eye type, and tail type (which depend on mutable properties)
- Implemented simple shaders to have sprites with a similar color palette for different hues.
- Implemented zoom capabilities
- Implemented the reproduction restrictions so that only species within the compatibility range reproduce

# Week 6 - Sensorial upgrades
- Implemented brain modules (sets of input and output neurons that creatures don't have by default but may evolve through mutations; i.e. pheromone emission/detection) in collaboration with Milan
- Worked on the visualization of the food density along the map using a shader (sadly even though the code is there this functionality was never made compatible with the zoom capabilities and it cannot be seen on the final project)
- Refactored the creature class into a set of smaller classes with more specific functionalities.
- Reworked the entire project to use shared pointers to entities in the heap instead of them being stored in vectors of the SimulationData class

# Week 7 - Final touches
- Helped merge with main the reproduction and attack projects after the creature refactor and the stack to heap change
- Implemented pheromones as a brain module
- Designed with Milan an algorithm based on the DBSCAN to cluster creatures into species and update it as the population changed. 




On top of all of this on a weekly basis I created guidelines for what each project was supposed to implement/accomplish and I worked closely with Nikola to decide what our next objectives in the road map were. Moreover, extensive hours were spent helping other members with questions about the objectives of specific implementations and finding hidden bugs all over the code.
