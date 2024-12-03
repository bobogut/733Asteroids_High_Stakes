// Relevant PROS library are used for declaring the controller. States header file simply defines enumators
#include "pros/misc.hpp"

#include "states.h"



enum Autons autonSelected;



bool findNextDown = false;
bool storeRing = false;



enum IntakeStates intakeState;
int intakeSpeed;
bool overrideIntakeState;



pros::Controller controller(pros::E_CONTROLLER_MASTER);