Week of Nov 27
Literature search on Neat neural network architecture to determine the specificities of our implementation.
Project setup and installing necessary dependencies to allow for it to run on my computer.
Defining problem space for neural network subproblems and identifying appropriate division of tasks, as well as homogeneity of implementations across the group.

Week of Dec 4
Implementation of add/remove link mutation function to introduce variability in population (better mimic nature). In the case of deletion, if the two random nodes chosen already have a link it is disabled. In the case of addition, if the two random nodes already have a disabled link it is enabled. Currently enabling/disabling is not used, but will allow for crossover later.
Implemented cycle checking as architecture currently does not support links that feed into themselves. DFS search allows us to identify cycles, so that future versions will control their presence rather than reject them.

Week of Dec 11
Implementation of add/remove neuron mutation which depended on the existence of other functions (link initialization with random parameter). Based on chosen implementation, added neurons are used to split an existing edge into two new edges. Currently added neurons are always in the hidden layer, this may change.
Worked on the food class to redesign the homogeneity of attributes across all food. Now split into meat and plants that have different lifespans and nutritional values. Meats lose value over time (mimicking rotting) while plants photosynthesize to increase in size and nutritional value.

Week of Dec 18
Implemented mutations in the activation functions of neurons. Initially stored as an attribute of neurons and updated upon mutation. Since we did not need to store this information, it changed to be an attribute of kNeurons, which are activated in the feed-forward system. Alignment (in terms of ID) is stored between these two neuron types.

Vacation
Implemented food differentiation for creatures. They have a mutable that dictates the ratio of energy yielded by plants or meats. Rather than observing food, they now find the closest plant or meat. Both passed as inputs for the neural network brain to induce differences in the evolution of behaviours between carnivores and herbivores.

Week of Jan 8
Implemented egg class to better mimic natural reproduction. Stores the mutables and genome of the creature to be hatched, and has properties of both food and creatures (can be eaten).
Changed the compatibility measure to better incorporate the presence of eggs. Currently reproduction is happening too frequently the offspring is spawned randomly. Now creatures have a sex (only females can lay eggs), and opposite sex creatures must be in close proximity to reproduce.
Rather than perpetually wishing to reproduce to pass on traits to future generations, creatures have an attribute that dictates reproduction desire. Female reproduction desire is a decreasing function in time that becomes null at a certain age (mutable). Similarly males have a decreasing function that is unbounded. Both genders can only reproduce after a certain age.

Week of Jan 15
Implemented exception handling functionality that both corrects flaws with scrolling/clicking on Mac and implemented additional information presentation functionality.
Currently the mouse position is taken relative to the user window. Due to a difference in the DPI across different systems, the coordinate used to iterate over creatures to handle clicking functionality introduced a misalignment. To differentiate between systems, adopted a new system on Mac that stores the upper left coordinate and updates it affinely upon panning/zooming. The specific window dimension of the user, as well as the currently displayed dimensions are compared, to determine the precise location of the mouse. Had to overcome many bugs due to lack of cross-platform compatibility (even for people on the same OS).
The scrolling logic that appeared to work correctly on Windows had to be changed for Mac to make it smoother, and to continue storing the correct current display of the screen. Zooming is now intuitively away from the mouse, rather than directly away from the centre. Led to further issues with clicking, as zooming now changes results in both a growing map as well as panning.

Week of Jan 22
Right clicking functionality has been introduced, which allows the user to follow the movement and information of a particular creature. The map is centred around the creature on every update and allows the user to track its movement while seeing its information panel as well as vision cone. Movement of the creature is averaged over 10 frames to smoothen visuals.
Updated zooming functionality on rebased branch with significantly different UI logic.

Week of Jan 29
Fixed errors with the saving/loading of files (uncaught exceptions such as cancelling save/load and invalid file names).
Implemented statistics saving in parallel with simulation data, which yields a text file with relevant values that correspond to the saved genomes/mutables.

Submission of final project.
