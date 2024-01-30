# Week 1

- Create project
- Setup CMake
- Implement the main Engine
- Link SFML and create QSFMLCanvas
- Implement thread-safe simulation data access
- Created a basic UI to control the engine
- Wrote detailed documentation on Notion on how to use the project base and what rules to follow

# Week 2

- Integrate the Google Testing framework
- Rewrite existing tests for the new framework
- Helped Ioana with designing the collision system
- Helped with the implementation of NEAT

# Week 3

- Fixed a segmentation fault when checking collisions (entities we checked for collisions with itself)
- Fixed QSFMLCanvas crashing on Linux
- Did some general code cleanup and minor fixes

# Week 4

- Tracked down a bug that caused random crashes at very specific circumstances (the issue was a rounding error of a double to something like -10e-19, which turned into NaN when using the square root, propagating the error into other parts of the code and eventually crashing). Took a very long time.
- Reworked cross-thread data access; created the DataAccessor object
- Refactored the SimulationData class into separate modules

# Week 5

- Tracked down a bug causing a crash upon creature death. The data wasn't getting locked properly, resulting in thread data access misalignment and a subsequent segmentation fault.
- Integrated QtCharts into the projects and created example graphs
- Fixed GitHub actions issues
- Reworked engine control flow from the UI (there were some memory leaks)

# Week 6

- Tracked down a bug causing a crash after about 30 minutes of runtime in release mode. The issue was cycles in neural networks infinitely multiplying a value, reaching infinity which propagated through other modules and eventually caused a crash in the collision system.
- Implemented a dynamic food density system, dependent on coordinates and with support for spawn rate
- Added debug and release configurations to CMake
- Moved config into a singleton
- Implemented settings loading from JSON
- Fixed some issues with graphs

# Week 7

- Reworked engine Run loop to prevent freezing and crashing when physics calculation time exceeds fixedDeltaTime
- Integrated species clustering into the UI, made it run on a separate thread and provided a thread-safe way to access its data
- Fixed the statistics saving system
- Spent a lot of time merging pull requests and finalizing the project
- Tinkered a lot with different simulation parameters to analyze different emerging behaviour of creatures

There were also a lot of small bugfixes that I didn't feel were worth mentioning individually, but they add up over time. Aside from all this, I regularly discussed design and implementation details with other team members, most notably Ioana, Carlos, and Milan. I constantly answered questions and provided help to whoever needed it.
