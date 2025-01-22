For tracking changes and going into greater detail after December 10, 2024.

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
- Messing around with functions for autonomous routines so that the path can be mirrored easily and without
too much human interaction (me when I forget a minus sign)

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