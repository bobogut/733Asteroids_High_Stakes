#include "pros/misc.hpp"

#include "states.h"



enum autons autonSelected;
bool autonRan;



bool findNextDown = false;
bool storeRing = false;



enum intakeStates intakeState;
int intakeSpeed;
bool overrideIntakeState;



pros::Controller controller(pros::E_CONTROLLER_MASTER);
