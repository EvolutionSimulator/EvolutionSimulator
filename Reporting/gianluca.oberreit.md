## Week 1
Set up the github repository.
Create Movable Entity class with movement with linear velocity (in the direction of the entity's orientation) and rotational velocity. Implement moving and changing orientation of creatures using this velcocities based on a timestep parameter.

## Week 2
Create git guidelines (on notion), set up correct settings for people, rebase branches. Be a git helpline.
Implement growing for the creatures.

## Week 3
Rebase or merge branches, amend git guidelines for easier implementation (no rebasing on main, only on project main branches), be a git helpline.
Add acceleration. Make the angle of acceleration and movement relative to the creature's orientation, not global coordinates.
Make the creature use accelerations instead of velocities to change their movement.
Add a frictional force that depends on size, velocity and angle of velocity with respect to orientation (strafing takes more energy than forward movement).
Refactor the codebase to follow google styleguide.
Start setting up automatic testing.

## Week 4
Be a git helpline.
Set up branch protection rules (1 approval to merge into main and **/main branches), require pull requests. Use the "CMake on multiple platforms" action and add necessary installations for ubuntu. Disable the windows tests for now. Enable mandatory passing of the checks when merging to main and **/main branches.

## Week 6
Start implementing logic for grabbing with a GrabbingEntity class. Creatures can grab 1 movable entity, and be grabbed by as many entities as can collide with them.
Start implementing logic for movement of an agglomeration of grabbing/grabbed creatures. This uses adding of creatures' intended forward accelerations scaled by their size for linear acceleration. For rotatational acceleration of the agglomeration, we create a couple of torques from every creature's intended rotational acceleration, project them perpendicular to the centre-of-agglomeration to centre of creature distancea and add the torques due to forward accelerations.
