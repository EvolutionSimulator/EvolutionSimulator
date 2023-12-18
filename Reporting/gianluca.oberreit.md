## Week 1
Create Movable Entity class and implement movement with velocity (in the direction of the entity's orientation) and rotational velocity.

## Week 2
Create grow function

## Week 3
Add acceleration. Make the angle of acceleration and movement relative to the creature's orientation, not global coordinates.
Make the creature use accelerations instead of velocities to change move.
Add a frictional force that depends on size, velocity and angle of velocity with respect to orientation (strafing takes more energy than forward movement).
Refactor the codebase to follow google styleguide.
Start setting up automatic testing.

## Week 4
Use the "CMake on multiple platforms" action and add necessary installations for ubuntu. Disable the windows tests for now. Enable mandatory passing of the checks when merging to main and **/main branches.
