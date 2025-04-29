#pragma once

// Relevant PROS library are used for declaring the controller. States header file simply defines enumators
#include <vector>

#include "pros/misc.hpp"

#include "my_includes/states.h"



namespace global {
    inline states::autonStates::Autons autonSelected = states::autonStates::None;
    inline bool initializedBrainCoords;

    inline bool runningAuton;
    inline bool ranAuton = false;
    inline bool runningOpControl;

    inline bool flipColorSort = false;
    inline bool overrideColorSort = false;
    inline bool overrideAntiJam = false;

    inline bool findNextDown = false;


    inline bool cancelArmMotion = false;

    inline bool test = false;

    inline enum states::intakeStates::IntakeStates intakeState = states::intakeStates::Resting;

    inline int ringsDetected = 0;


    inline enum states::armStates::ArmStates armState = states::armStates::Resting;
    inline std::vector<states::armStates::ArmStates> armStatesQueue;
}

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);
