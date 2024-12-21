For tracking changes and going into greater detail after December 10, 2024.

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