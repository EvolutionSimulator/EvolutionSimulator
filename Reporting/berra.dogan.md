1st week (20.11.2023 - 27.11.2023)

While the team leader was setting up the system, I researched possible ideas, thought about the possible tasks that I would be willing to do, and joined the NEAT Algorithm Implementation and Physics Engine teams.

2nd week (27.11.2023 - 04.12.2023)

The first half of the week I programmed the remove neuron and remove link mutations. Further, I implemented disable/enable neuron and link functions that would be necessary in the future to implement recessive/dominant traits. Later, I met Ioana to discuss the collision system. We first determined the base functions necessary to test if two objects were intersecting  (circle-circle, circle-line collisions). Then since we are expecting entities to change directions and interact with many others, we thought that it would be more efficient to find an algorithm that avoids looping through each combination of two objects (O(n^2)). We looked through options Sweep and Prune Algorithm, R-tree, and Spatial Partitioning, and decided at the end that Spatial Partitioning would be the best option for more practical utilization. Then, we partitioned the work and I programmed the collision tests and the related sub functions.

3rd week (04.12.2023 - 11.12.2023)

I have modified my collision functions from last week according to the new tests. Further, I updated the grid system (spatial partitioning). Before my modification, the program was only checking collisions of entities that have centers in the same grid. Now, depending on the size of the objects, it checks more of the neighboring grids and neighboring grids to neighboring grids and so on, and function ensures all possible collisions are detected with as few comparisons as possible. Moreover, I modified the pre-existing variables to store pointers to objects rather than the objects themselves for maintenance purposes. I have modified and added functions in the system-links/grid-update branch. However due to the the crash of UI, the changes I added were committed through Ioana’s “Vector Grid and Fixed Bugs” and  “Merge pull request #53 from EvolutionSimulator/system-links/fix-undorder…” commits. To be more specific, I wrote CheckCollisions and GetNeighbours functions from scratch and modified all the other functions in simulationdata to work with vectors instead of unordered maps and pointers instead of objects.

4th week (11.12.2023 - 18.12.2023)
I have researched how a torus map is implemented and adjusted GetNeighbours and GetDistance to work for the torus map. Further, I have reviewed my pre-existing code and improved the efficiency of the RemoveNeuron function.
