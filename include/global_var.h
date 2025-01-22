#pragma once

// Relevant PROS library are used for declaring the controller. States header file simply defines enumators
#include "pros/misc.hpp"
#include "states.h"



namespace global {
    inline Autons autonSelected;
    inline bool initializedBrainCoords;

    inline bool runningAuton;
    inline bool runningOpControl;

    inline bool flipColorSort = false;
    inline bool overrideColorSort = false;

    inline bool findNextDown = false;
    inline bool storeRing = false;
    inline bool overrideIntakeState = false;

    inline enum IntakeStates intakeState = INTAKERESTING;
}

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);
