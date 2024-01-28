# Week 4-8 December
Implemented the mutation of link weights in a method `MutateChangeWeight()`, which changes the weight of the link between two neurons using normal distribution. The mutation rate is kept between a certain interval in order to ensure future compatibility between individuals.

# Week 18-22 December
Implemented computing the compatibility between two individuals in a method `ComputeCompatibilityGenomes()`, which compares the genomes of the current individual and another one which is given as an argument. The method compares the two genomes in order to find similarities (links, neurons) in order to calculate and return a value which represents the compatibility between the two individuals.

# Week 22-26 January
Created a function for saving the state of a simulation such as current settings (map size, friction coefficient, food density and creature density), current food entities with their types and characteristics. For saving entities like eggs and creatures I needed to also save the genome which is stored as a neural network in the program. In order ro be able to save it and rebuild it later I decomposed it into neurons with their specific type and id; and the links between them with in and out id and weight.
For retrieving the data from a simulation I reversed the process, giving the parameters of the simulation the values that are stored in a file. We reconstruct the genome of the creatures using the neurons and the links.
Both functions use a OS-specific folder for storing and retrieving data in order to have the way data is sored in line with other priograms.
