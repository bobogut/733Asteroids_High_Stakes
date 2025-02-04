// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include <iostream>
#include <ostream>

#include "global_var.h"
#include "main.h"

#include "ports.h"
#include "pros/colors.hpp"
#include "pros/misc.h"
#include "robot_functions.h"
#include "states.h"
#include "brain_screen.h"



/*
9/12

11/12 needed
New buttons: 

a (doinker),
l1 (intake toggle),
r1 (mogo),
y (reverse drive),
up (outtake),
r2 (wall stake if primed, primed if wall stake),
l2 (prime lb if rest, rest lb if extended/scored),
down (tip),
b (alliance stake),
? (color sort override),
? (down position)
*/


void updateIntakeStates() {
    // All the code for controllering the intake state

    // When the L1 button is pressed 
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        // Toggle the intake to the mogo or resting state with the same button
        if (global::intakeState == states::intakeStates::Resting || global::intakeState == states::intakeStates::Reverse)
           global::intakeState = states::intakeStates::Mogo;
        else
           global::intakeState = states::intakeStates::Resting;

        std::cout << global::intakeState << std::endl;
    } 
    
    /*
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // When the R2 button is pressed set the intake to the reverse state
       global::intakeState = states::intakeStates::Reverse;                                       
    else if (global::intakeState != states::intakeStates::Mogo) // If the last state was reverse and the R2 button was let go, set the intake to its resting state
       global::intakeState = states::intakeStates::Resting;
    */

    /*
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) // When the down button is pressed toggle the color sorter override
        global::flipColorSort = !global::flipColorSort;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) // When the up button is pressed flip the color the color sorter looks for
        global::overrideColorSort = !global::overrideColorSort;
    */


    /*
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) // When the up button is pressed flip the color the color sorter looks for
        global::storeRing = true;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) // When the B button is pressed find the next valid "down" position
        global::findNextDown = true;
    */
}



void updateArmStates() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (global::armState == states::armStates::AllianceStake || global::armState == states::armStates::TipMogo || global::armState == states::armStates::WallStake)
            global::armStatesQueue.push_back(states::armStates::Resting);
        else if (global::armState == states::armStates::Resting)
            global::armStatesQueue.push_back(states::armStates::PrimeOne);
        else
            global::armStatesQueue.push_back(states::armStates::PrimeTwo);
    } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
        if (global::armStatesQueue.size() != 0) {

            std::cout << "Case 1" << std::endl;
            

            if (global::armStatesQueue.end()[-1] == states::armStates::WallStake)
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::WallStake);
        } else {
            std::cout << "Case 2" << std::endl;

            if (global::armState == states::armStates::WallStake)
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::WallStake);
        }

        std::cout << "Arm state queue is " << global::armStatesQueue.begin()[0] << std::endl;

        for (int i = 0; i < global::armStatesQueue.size(); i++)
            std::cout << global::armStatesQueue.begin()[i];

        std::cout << "" << std::endl;
    } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        global::armStatesQueue.push_back(states::armStates::AllianceStake);
    } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        global::armStatesQueue.push_back(states::armStates::TipMogo);
    }

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



    global::intakeState = states::intakeStates::Resting;



    setBrainImage();

    /*
    if (!global::initializedBrainCoords) {
        

        global::initializedBrainCoords = true;
    }
    */

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;
    


    while (true) {
        /*
        // optical.set_led_pwm(100); // Turns on the LED in the optical sensor to ensure that lighting on the ring is consistent



        updateIntakeStates();
        


        // handlePneumatics();

        std::string clampOpen;

        if (mogoClamp.is_extended())
            clampOpen = "Closed";
        else
            clampOpen = "Opened";

        // controller.set_text(0, 0, clampOpen); // To inform the driver whether or not the clamp is opened or closed when they cannot see it clearly


        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When R2 is pressed toggle the reverse state
            reverse = !reverse;

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            global::armState = states::armStates::TipMogo;
            // std::cout << "Here notice me " << global::armState << std::endl;
        }


        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            global::armState = states::armStates::Resting;
            std::cout << "notice me" << std::endl;
        }

        

        handleBase(reverse);
        */

        updateArmStates();

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
            std::cout << "UP" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
            std::cout << "DOWN" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
            std::cout << "LEFT" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
            std::cout << "RIGHT" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
            std::cout << "A" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
            std::cout << "B" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
            std::cout << "Y" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
            std::cout << "X" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
            std::cout << "L1" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
            std::cout << "L2" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
            std::cout << "R1" << std::endl;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
            std::cout << "R2" << std::endl;

        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    // optical.set_led_pwm(0);



    global::runningOpControl = false;
}