// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include "global_var.h"
#include "main.h"

#include "ports.h"
#include "pros/misc.h"
#include "robot_functions.h"
#include "states.h"
#include "brain_screen.h"

#include "lemlib/api.hpp"
#include <iostream>
#include <ostream>
#include <string>


void updateIntakeStates() {
    // All the code for controllering the intake state
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) // When the L1 button is pressed
    {
        // Toggle the intake to the mogo or resting state with the same button
        if (global::intakeState == INTAKERESTING || global::intakeState == INTAKEREVERSE)
           global::intakeState = IntakeMogo;
        else
           global::intakeState = INTAKERESTING;

        std::cout << global::intakeState << std::endl;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // When the R2 button is pressed set the intake to the reverse state
       global::intakeState = INTAKEREVERSE;                                       
    else if (global::intakeState != IntakeMogo) // If the last state was reverse and the R2 button was let go, set the intake to its resting state
       global::intakeState = INTAKERESTING;



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) // When the down button is pressed toggle the color sorter override
        global::flipColorSort = !global::flipColorSort;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) // When the up button is pressed flip the color the color sorter looks for
        global::overrideColorSort = !global::overrideColorSort;



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) // When the up button is pressed flip the color the color sorter looks for
        global::storeRing = true;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) // When the B button is pressed find the next valid "down" position
        global::findNextDown = true;
}



void handlePneumatics() {
    // When the R1 button is pressed toggle the mogo clamp
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {

        if (mogoClamp.is_extended()) // If the clamp is retracted, reverse the intake to a neutral position
            global::findNextDown = true;
        
        mogoClamp.toggle();
    }



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) // When the A button is pressed toggle the doinker
        doinker.toggle();
}



void opcontrol() {
    global::runningOpControl = false;
    global::runningOpControl = true;
    global::runningAuton = false;



    global::intakeState = INTAKERESTING;



    // setBrainImage();

    if (!global::initializedBrainCoords) {
        

        global::initializedBrainCoords = true;
    }

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;
    


    while (true) {
        // optical.set_led_pwm(100); // Turns on the LED in the optical sensor to ensure that lighting on the ring is consistent



        updateIntakeStates();
        


        handlePneumatics();

        std::string clampOpen;

        if (mogoClamp.is_extended())
            clampOpen = "Closed";
        else
            clampOpen = "Opened";

        // controller.set_text(0, 0, clampOpen); // To inform the driver whether or not the clamp is opened or closed when they cannot see it clearly


        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When R2 is pressed toggle the reverse state
            reverse = !reverse;

        handleBase(reverse);



        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    // optical.set_led_pwm(0);



    global::runningOpControl = false;
}