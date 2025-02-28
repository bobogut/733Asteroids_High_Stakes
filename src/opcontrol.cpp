// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include "main.h"

#include "my_includes/global_var.h"
#include "my_includes/ports.h"
#include "my_includes/robot_functions.h"
#include "my_includes/states.h"
#include "my_includes/brain_screen.h"
#include "pros/misc.h"



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



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) // When the up button is pressed flip the color the color sorter looks for
        global::overrideColorSort = !global::overrideColorSort;
}



void updateArmStates() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        if (global::armStatesQueue.size() != 0) {
            if (global::armStatesQueue.end()[-1] == states::armStates::AllianceStake || global::armStatesQueue.end()[-1] == states::armStates::WallStake
                || global::armStatesQueue.end()[-1] == states::armStates::TipMogo || global::armStatesQueue.end()[-1] == states::armStates::PrimeTwo)
                global::armStatesQueue.push_back(states::armStates::Resting);
            else if (global::armStatesQueue.end()[-1] == states::armStates::Resting )
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeTwo);
        } else {
            if (global::armState == states::armStates::AllianceStake || global::armState == states::armStates::TipMogo
                || global::armState == states::armStates::WallStake || global::armState == states::armStates::PrimeTwo)
                global::armStatesQueue.push_back(states::armStates::Resting);
            else if (global::armState == states::armStates::Resting )
                global::armStatesQueue.push_back(states::armStates::PrimeOne);
            else
                global::armStatesQueue.push_back(states::armStates::PrimeTwo);
        }
    } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (global::armState != states::armStates::TipMogo) {
            global::armStatesQueue.clear();

            global::armStatesQueue.push_back(states::armStates::TipMogo);
        } else
            global::armStatesQueue.push_back(states::armStates::Resting);
    } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        if (global::armState != states::armStates::AllianceStake) {
            global::armStatesQueue.clear();

            global::armStatesQueue.push_back(states::armStates::AllianceStake);

            global::armStatesQueue.push_back(states::armStates::Resting);
        }
    }


    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        global::armStatesQueue.clear();

        if (global::armState != states::armStates::WallStake)
            global::armStatesQueue.push_back(states::armStates::WallStake);
    } else if (global::armState == states::armStates::WallStake) {
        if (global::armStatesQueue.size() == 0)
            global::armStatesQueue.push_back(states::armStates::PrimeOne);
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
        optical.set_led_pwm(100); // Turns on the LED in the optical sensor to ensure that lighting on the ring is consistent



        updateIntakeStates();

        updateArmStates();
        


        handlePneumatics();

        std::string clampOpen;

        if (mogoClamp.is_extended())
            clampOpen = "Closed";
        else
            clampOpen = "Opened";

        // controller.set_text(0, 0, clampOpen); // To inform the driver whether or not the clamp is opened or closed when they cannot see it clearly


        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When B is pressed toggle the reverse state
            reverse = !reverse;



        handleBase(reverse);



        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    optical.set_led_pwm(0);



    global::runningOpControl = false;
}