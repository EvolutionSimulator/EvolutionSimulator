# Week 4-8 December
Implemented the mutation of link weights in a method `MutateChangeWeight()`, which changes the weight of the link between two neurons using normal distribution. The mutation rate is kept between a certain interval in order to ensure future compatibility between individuals.

# Week 18-22 December
Implemented computing the compatibility between two individuals in a method `ComputeCompatibilityGenomes()`, which compares the genomes of the current individual and another one which is given as an argument. The method compares the two genomes in order to find similarities (links, neurons) in order to calculate and return a value which represents the compatibility between the two individuals.
