First set of Commits: I implemented pause and restart functionalities for the simulation.

Second set of Commits: I worked on adding a slider to adjust the simulation friction constant, with an interactive label. This slider will mostly be used to test the effects of movement friction in the simulation.

Third set of Commits: I joined the start and Pause button into a singular button which can be toggled.

Fourth set of Commits: I implemented my first example graph which tracked the number of creatures in the simulation over the time elapsed. It does not yet fully work.

Fifth set of Commits: I used QtCharts to re-make the previous graph rather than building graph logic from scratch.

Sixth set of Commits: I implemented a new graph which graphs the average creature size over time, and edited the formatting of the friction slider.

Seventh set of Commits: I worked on button design making the restart button with pixel art (with piskel) in order to create a more unified look for the UI. I also edited the settings and pause/restart button files to make the project more uniform.

Eighth set of Commits: I added many more simple graphs, after the refractor by Nikola, and I added a Scatterplot graph which displays individual creatures as points in a plane based on their size and energy. I also improved the dropdown menu’s functionality. Graphs are now drawn with template functions in order to reduce writing unnecessary code.

9th set of commits: I added more scatter plot graphs, and the feature that when a creature is selected it appears as red on the scatterplot. I also removed the friction slider from the mainwindow, and improved the one in the configuration window. I made it live update values when moved, and I made the simulation speed slider “continuous” with updating values real time.

10th set of commits: I added the functionality to save graphs as csv and png files on one’s desktop. If the folder EvolutionSimulationData doesn’t exist, we create one and store the files there.

11th set of commits: Made a new stacked area plot graph to display evolution of species population over time.
