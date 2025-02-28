For tracking changes and going into greater detail after December 10, 2024.

Feb 25, 2025
- Reimplemented color sort, calculations changed due to new sprocket size
- Changed calculations to use integers rather than floats due to using degrees rather than rotations

Feb 22, 2025
- Got a basic 3 ring auton for quals and a 4 ring auton for elims

Feb 20-21, 2025
- Worked on skills path

Feb 18, 2025
- Created a queue system for holding ladybrown arm states
- Got the ladybrown PID tuned
- Added extra timeouts to avoid the jittering motion/burning out the motor

Feb 11, 2025
- Implemented code for just running the first stage now that our intake runs on 16.5W, mainly for storing rings in autonomous

Feb 5, 2025
- Removed the brake from the color sort to keep momentum on the ring
- Moved my declerations to my_includes so they are easier to find
- Made store ring and find next down intake states for consistency

Feb 3, 2025
- Merged MyPose class with LemLib Pose class for less clutter
- Created a queue system for the ladybrown to decrease downtime
- Renamed ring storing related enums for ladybrown to "Prime"
- Fixed a bug with timeout not being properly tracked

Feb 2, 2025
- Created a parent class for all the coordiante info as they are all look to do the same thing  
- Removed the need for the coordinateInfo namespace by making the child classes have distinct names from their LemLib counterparts
- Added anti-jam code to the intake

Jan 31, 2025
- Created my own PID class to have an easier time tracking early exit range
- Updated the project to Pros 4.1.1 and LemLib 0.5.5

Jan 30, 2025
- Started tuning arm PID and finding arm positions

Jan 28, 2025
- Created a namespace for states for clarity
- Laid the groundwork for the ladybrown code (arm states, arm function, etc.)
- Placed coordiante classes into their own namespace, mainly to differenetiate my pose from lemlib::Pose
- Made reflecting coordinates for autonomous a class function

Jan 21, 2025
- Moved some function defintions from robot funcitons to opcontrol, may move them back if I figure better categorization later
- Disabled color sorter for Jan 19, 2025 competition as we did not have the optical sensor then
- PID constants need retuning as the bot has been rebuilt, lateral tracking wheel has been removed and replaced with chassis IMEs
- Merged intake motors into a motor group, though it will be transformed into a single motor upon the construciton of a lady brown
- Added an image to the brain for some cosmetic fun
- Started laying the groundwork for writing to a micro SD card for a robotics-related coding project

Jan 6, 2025
- Created various classes (Point and Swing) to make storing information easier

Dec 31, 2024
- Created a global variable namespace so things are clearing in the code
- Messing around with functions for autonomous routines so that the path can be mirrored easily and without too much human interaction (me when I forget a minus sign)

Dec 31, 2024 (technically Dec 21, just forgot to commit, tournament changes):
- Fixed autonomous paths
- Deleted unnecessary brackets
- Removed error timeouts for lateral movement as it was messing with the autonomous and we didn't have time to fix it

Dec 20, 2024:
- Changed port number for horizontal tracking wheel
- Fixed bug for color sorter if statement
- Updated negative corner autonomous to be a ring rush

Dec 13, 2024:
- Reversed horizontal tracking wheel as it was tracking incorrectly
- Finished tuning anti-windup and KI, undershoot was actually caused by a faulty LemLib decleration measuring 2" omniwheels as 2.125"
- Printed position to brain screen to debug LemLib (was committed a bit late)
- Created the boolean initialziedBrainCoords to avoid initializing it twice

Dec 11, 2024:
- Tuned anti-windup and kI to attempt to solve undershoot
- Added some paths in autonomous to test the robot's accuracy