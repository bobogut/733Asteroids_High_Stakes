#pragma once

// Relevant PROS library are used for declaring the controller. States header file simply defines enumators
#include "pros/misc.hpp"
#include "states.h"



extern enum Autons autonSelected;



extern bool findNextDown;
extern bool storeRing;



extern enum IntakeStates intakeState;
extern int intakeSpeed;
extern bool overrideIntakeState;



extern pros::Controller controller;
