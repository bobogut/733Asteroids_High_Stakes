// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include "main.h"

#include "my_includes/global_var.h"
#include "my_includes/ports.h"
#include "my_includes/robot_functions.h"
#include "my_includes/states.h"
#include "my_includes/brain_screen.h"
#include "pros/misc.h"

#include "lemlib/chassis/chassis.hpp"


void updateIntakeStates() {
    // All the code for controllering the intake state

    // When the L1 button is pressed 
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        // Toggle the intake to the mogo or resting state with the same button
        if (global::intakeState == states::intakeStates::Resting || global::intakeState == states::intakeStates::Reverse)
           global::intakeState = states::intakeStates::Mogo;
        else
           global::intakeState = states::intakeStates::Resting;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // When the R2 button is pressed set the intake to the reverse state
       global::intakeState = states::intakeStates::Reverse;      
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
        global::intakeState = states::intakeStates::Arm;
    else if (global::intakeState != states::intakeStates::Mogo && global::intakeState != states::intakeStates::Arm)
        global::intakeState = states::intakeStates::Resting;                                 



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) // When the up button is pressed flip the color the color sorter looks for
        global::overrideColorSort = !global::overrideColorSort;

    
}



void updateArmStates() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        /*
        if (global::armStatesQueue.size() != 0) {
            if (global::armStatesQueue.end()[-1] == states::armStates::Resting || global::armStatesQueue.end()[-1] == states::armStates::PrimeTwo)
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeTwo);
        } else {
            if (global::armState == states::armStates::Resting || global::armState == states::armStates::PrimeTwo)
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeTwo);
        }
        */
        if (global::armState != states::armStates::PrimeOne)
            global::armStatesQueue.push_back(states::armStates::PrimeOne);
        else
            global::armStatesQueue.push_back(states::armStates::Resting);
    }
    
    
    
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        global::armStatesQueue.clear();

        global::cancelArmMotion = true;

        global::armStatesQueue.push_back(states::armStates::Resting);

    }




    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        global::armStatesQueue.clear();

        if (global::armState != states::armStates::TipMogo)
            global::armStatesQueue.push_back(states::armStates::TipMogo);

        std::cout << "Attempting to prime size is " << global::armStatesQueue.size() << " " << global::armStatesQueue.end()[-1] << std::endl;

    } else if (global::armState == states::armStates::TipMogo && global::armStatesQueue.size() == 0)
        global::armStatesQueue.push_back(states::armStates::Resting);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        global::armStatesQueue.clear();

        if (global::armState != states::armStates::AllianceStake)
            global::armStatesQueue.push_back(states::armStates::AllianceStake);
    } else if (global::armState == states::armStates::AllianceStake) {
        if (global::armStatesQueue.size() == 0) {
            if (global::autonSelected == states::autonStates::Skills)
                global::armStatesQueue.push_back(states::armStates::Resting);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
        }
    }


    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        global::armStatesQueue.clear();

        if (global::armState != states::armStates::WallStake)
            global::armStatesQueue.push_back(states::armStates::WallStake);
    } else if (global::armState == states::armStates::WallStake) {
        if (global::armStatesQueue.size() == 0) {
            if (global::autonSelected == states::autonStates::Skills)
                global::armStatesQueue.push_back(states::armStates::Resting);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
        }
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



    // setBrainImage();

    if (!global::initializedBrainCoords) {
        robotCoordinates();

        global::initializedBrainCoords = true;
    }

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;

    /*
    if (!global::ranAuton) {
        if (global::autonSelected == states::autonStates::Skills) {

            base.setPose(-34, -28, 270);

            base.turnToPoint(-56, -8.5, 5000);

            base.moveToPoint(-56, -8.5, 5000);

            base.turnToPoint(-69.875, 0, 5000);

            base.waitUntilDone();

            pros::delay(5000);

            base.turnToHeading(360-41, 5000);

        }
    }
    */


    base.setPose(24, -24, 270);

    // -53, -24.6, 242.5

    // 19.8, -43.3, 237


    while (true) {
        // std::cout << "HELLO IN OP" << std::endl;

        optical.set_led_pwm(100); // Turns on the LED in the optical sensor to ensure that lighting on the ring is consistent





        





        updateIntakeStates();

        updateArmStates();
        


        handlePneumatics();

        std::string clampOpen;

        if (mogoClamp.is_extended())
            clampOpen = "closed";
        else
            clampOpen = "opened";

        controller.set_text(0, 0, "Clamp is " + clampOpen); // To inform the driver whether or not the clamp is opened or closed when they cannot see it clearly



        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) // When B is pressed toggle the reverse state
            global::overrideAntiJam = !global::overrideAntiJam;




        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When B is pressed toggle the reverse state
            reverse = !reverse;



        handleBase(reverse);



        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    optical.set_led_pwm(0);



    global::runningOpControl = false;
}