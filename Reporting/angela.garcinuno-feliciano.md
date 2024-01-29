1st week (20.11.2023 - 27.11.2023)

Created creature energy and creature states (Alive / Dead). Created the update energy function, which decreases the creatures energy at each simulation update depending on its size and speed, as well as the basic functions (get, change).

2nd week (27.11.2023 - 04.12.2023)

Added a Health attribute to creatures that replaces some of the properties of Energy. Creatures now have health (initialized at 100). Health and energy are limited at 100 and they can be converted from one to another when they reach a certain threshold. I modified the death condition so it depends on health instead of energy.

3rd week (04.12.2023 - 11.12.2023)

Fixed some bugs in health and energy, and added basic test functions. I also added age to creatures, which is initialized at 0 and increased at each time interval (simulation update). I implemented a feature where the creature's age limits it's max energy.

4th week (11.12.2023 - 18.12.2023)

Implemented the feature of creatures becoming meat when they die, adding a Meat object with the corresponding size and nutritional value attributtes to the food vector and removing the dead creature from the creature vector in the most efficient way I could.

5th week (08.01.2024 - 14.01.2024)

Added Health and Energy icons to the UI. Added eggs to the food vector and to the UI. When trying to implement the reproduction system I started by creating an egg object with the necessary attributes when the creatures reproduce, adding them to the grid and the UI... However the code structure gave rise to multiple problems, such as eggs being eaten in bites, with the eggs size decreasing and specially when hatching the eggs, since at the time egg inherited from creature and food, and was in the food vector, and when hatching we had to remove them from the food vector and add them to the creature vector as a creature object, not an egg. Part of the object information was lost in the process so we decided to restructure most of this part of the code.


6th week (15.01.2024 - 21.01.2024)

Created configuration screen and added a button to open it from the main window. Removed the creature density, food density and friction sliders from the main window and placed sliders to edit them from the configuration window, connecting the sliders to change the values in the simulation.

7th week (22.01.2024 - 28.01.2024)

Improved the configuration window UI and prepared for the project presentation.
