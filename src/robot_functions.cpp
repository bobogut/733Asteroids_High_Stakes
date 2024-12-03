// The LemLib chassis library used for chassis movement. Relevant PROS library for controller inputs, optical outputs, motor control, and pneumatic control (embeded in various other 
// includes). We are using the Using standard C++ array library to easily return information and the standard C++ math library to do calculations.
#include <array>
#include <cmath>

#include "lemlib/chassis/chassis.hpp"

#include "global_var.h"
#include "ports.h"
#include "pros/misc.h"
#include "states.h"



bool overrideColorSort = false;
bool flipColorSort = false;

// Top hook positions: 1st = 2.0 / 6.0, 2nd = 17.0 / 6.0, 3rd = 33.0 / 6.0
// Neural hook position: 1st = 0.0, 2nd = 15.0 / 6.0, 3rd = 31.0 / 6.0
// The hooks should be at the same positions every 46.0 / 6.0 rotations or 7.6666... rotations (Numbers are based on chains over knobs on a sprocket)

// Function to find potential hook positions where they are in a "top position" (i.e. where hooks are at their peak)
std::array<double, 3> nextTopHook() {
    double fullRotations = std::floor(intakeHook.get_position() * 6.0 / 46.0); // Used to find how many full rotations have been completed

    if (intakeHook.get_position() * 6.0 / 46.0 - fullRotations >= (33.0 / 46.0)) // Only relevant to bringing a hook to its peak. If the hooks have not completed a full rotation yet the
        fullRotations += 1;                                                      // the third hook has already passed its peak we have to look at the next rotation otherwise the intake
                                                                                 // goes backwards. When there is a remainder of 33 / 46 or 0.71739... (percentage of the rotation travelled 
                                                                                 // after the third hook) we know this is the case.

    double firstHookPosition = (2.0 / 6.0) + (46.0 / 6.0) * fullRotations; // Math for calculating where the hooks could be. The logic is explained above.
    double secondHookPosition = (17.0 / 6.0) + (46.0 / 6.0) * fullRotations;
    double thirdHookPosition = (33.0 / 6.0) + (46.0 / 6.0) * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition}; // Returns an array of the potential positions for use elsewhere in the code
}

// Function to find potential hook positions where they are in a "neutral position" (i.e. where hooks are relatively parallel to the ground to avoid the first rung of the ladder). All of
// the logic is the same as the logic used for finding potential top hook
std::array<double, 3> nextNeutralHook() {
    double fullRotations = std::floor(intakeHook.get_position() * 6.0 / 46.0); 

    double firstHookPosition = (46.0 / 6.0) * fullRotations;
    double secondHookPosition = (15.0 / 6.0) + (46.0 / 6.0) * fullRotations;
    double thirdHookPosition = (31.0 / 6.0) + (46.0 / 6.0) * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition};
}

// For clarity when I am writing the code
enum DesiredHookPositions {
    TOP,
    NEUTRAL
};

double findClosestHook(DesiredHookPositions desiredHookPosition) {
    double desiredPosition;                   // Where we want the intake to move to
    std::array<double, 3> potentialPositions; // An array for storing all potentially viable hook positions

    if (desiredHookPosition == TOP) {
        potentialPositions = nextTopHook();

        // Runs through every item of the potential positions array
        for (int i = 0; i < potentialPositions.size(); i++) {
            if (potentialPositions[i] - intakeHook.get_position() > 0) // If the value is negative we know that the hook associated with the potential position has already passed the 
                desiredPosition = potentialPositions[i];               // optical sensor and thus cannot hold the ring. As the potential positions array is arranged from smallest to
                                                                       // largest, we can confidently assume that the first valid position matches to the correct hook.
                    
        }
        
    }
    else if (desiredHookPosition == NEUTRAL) {
        potentialPositions = nextNeutralHook();
        
        for (int i = 0; i < potentialPositions.size(); i++) {
            if (potentialPositions[i] - intakeHook.get_position() <= 0) // If the value is positive we know that the hook associated with the potential position needs to move forward to   
                desiredPosition = potentialPositions[i];                // reach a "neutral position". As we want the intake to move backwards to avoid catching on mogos, we can 
                                                                        // immediately discard it. The equal check is to catch when the intake has not moved yet we accidently call it to 
                                                                        // reach a neutral position to avoid breaking the programming. As stated earlier the potential positions array is                                                                         // 
                                                                        // arragned from smallest to largest, however the logic this time is that the last valid position should match to
                                                                        // the correct hook as it would be the closest to the current position without having the intake move forwards.
        }
    }

    return desiredPosition;
}

void moveIntakeToDesiredPosition(DesiredHookPositions desiredHookPosition) {
    double desiredPosition = findClosestHook(desiredHookPosition); // Find out where the intake needs to rotate to in order to properly position a hook.

    intakeHook.move_absolute(desiredPosition, 600); // Starts rotating the intake towards the position.

    while (std::abs(desiredPosition - intakeHook.get_position()) > 0.0125) // While error is greater than 0.0125 keep waiting 5 milliseconds to give the move_absolute function time.
        pros::delay(5);

    if (desiredHookPosition == TOP) // If we want to throw a ring off (color sort) then we want a delay to ensure the ring's momentum properly carries it off the hooks
        pros::delay(500);
}

void updateIntakeStates() {
    // All the code for controllering the intake state
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) // When the L1 button is pressed
    {
        // Toggle the intake to the mogo or resting state with the same button
        if (intakeState == INTAKERESTING || intakeState == INTAKEREVERSE)
            intakeState = INTAKEMOGO;
        else
            intakeState = INTAKERESTING;
    }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // When the R2 button is pressed set the intake to the reverse state
        intakeState = INTAKEREVERSE;                                       
    else if (intakeState != INTAKEMOGO) // If the last state was reverse and the R2 button was let go, set the intake to its resting state
        intakeState = INTAKERESTING;

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) // When the down button is pressed toggle the color sorter override
        flipColorSort = !flipColorSort;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) // When the up button is pressed flip the color the color sorter looks for
        overrideColorSort = !overrideColorSort;
    
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) // When the up button is pressed flip the color the color sorter looks for
        storeRing = true;

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) // When the B button is pressed find the next valid "down" position
        findNextDown = true;
}

bool checkForColor(bool opposite) {
    if (autonSelected == REDPOSITIVECORNER || autonSelected == REDNEGATIVECORNER || autonSelected == SKILLS) {
        // If apart of the red alliance or selected the skills autonomus routine
        if (opposite) // Check for blue rings which have an approximate hue range of 167-210
            return optical.get_hue() > 167;
        else // Check for red rings which have an approximate hue range of 15-18
            return optical.get_hue() < 18;
    }
    else if (autonSelected == BLUEPOSITIVECORNER || autonSelected == BLUENEGATIVECORNER)  {
        // Same logic as above but vice versa
        if (opposite)
            return optical.get_hue() < 167;
        else
            return optical.get_hue() > 18;
    }

    return false;
}

void handleIntake() {
    /*
    Variables for debugging

    double highestHue = 18;
    double lowestHue = 17;
    */

    while (true)
    {
        /* 
        Loggers for debugging

        if (optical.get_hue() > highestHue)
            highestHue = optical.get_hue();
        else if (optical.get_hue() < lowestHue && optical.get_hue() != 0)
            lowestHue = optical.get_hue();

        std::cout << "Color is " << optical.get_hue() << ", time is " << pros::millis() << std::endl; // For figuring out color
        std::cout << "Lowest hue is " << lowestHue << std::endl;
        std::cout << "Highest hue is " << highestHue << std::endl;
        */

        // If the driver has not overriden the color sort, look for opposing rings to throw off at the top.
        if (checkForColor(!flipColorSort) && !overrideColorSort) {
            intakeHook.move_velocity(0); // Stops the intake so that all necessary calcuations can occur

            moveIntakeToDesiredPosition(TOP); // Moves a hook to its top position
        }

        // Same logic as the color sorter except for moving a hook to its neutral position
        if (findNextDown) { 
            intakeHook.move_velocity(0);

            moveIntakeToDesiredPosition(NEUTRAL);

            findNextDown = false;
        }

        // This is for storing a ring in the intake when moving towards a goal. Should help with making routes in autonomous and driver controller more convenient 
        if (storeRing) {
            intakePre.move_velocity(600);
            intakeHook.move_velocity(459);

            while (!checkForColor(false)) // Wait for a ring to come into the range of the color sensor
                pros::delay(5);

            // Stop the intake
            intakePre.move_velocity(0);
            intakeHook.move_velocity(0);

            storeRing = false;
        }



        if (intakeState == INTAKEMOGO) // Sets the intake to 600 rpm, running directly to the mogo
            intakeSpeed = 600;
        else if (intakeState == INTAKERESTING) // Stops the intake
            intakeSpeed = 0;
        else if (intakeState == INTAKEREVERSE) // Reverses the intake at 600 rpm
            intakeSpeed = -600;
        
        

        if (!overrideIntakeState) {
            intakePre.move_velocity(intakeSpeed);              // Runs the first stage intake at whatever rpm was set
            intakeHook.move_velocity(intakeSpeed * 459 / 600); // Runs the second stage intake at a slightly slower speed to ensure rings are placed on the mogo properly
        }



        pros::delay(5); // A delay to ensure all processes run smoothly
    }
}



void handlePneumatics() {
    // When the R1 button is pressed toggle the mogo clamp
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {

        if (mogoClamp.is_extended()) // If the clamp is retracted, reverse the intake to a neutral position
            findNextDown = true;
        
        mogoClamp.toggle();
    }



    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) // When the A button is pressed toggle the doinker
        doinker.toggle();
}



void handleBase(bool reverse) {
    lemlib::ExpoDriveCurve driveCurve(10, 20, 1.008); // Implements a deadband to prevent drift and a scaler for more acute motion

    int leftVelocity = 0; // Velocity for the left and right side of the chassis respectively
    int rightVelocity = 0;

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When R2 is pressed toggle the reverse state
            reverse = !reverse;

    if (!reverse) {
        // Regular chassis controls
        leftVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        rightVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
    }
    else {
        // When the chassis is reversed
        leftVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * -1); // Invert the controllers and swap the sticks
        rightVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * -1); // (i.e., left now controls rght and vice versa)
    }


    /*
    std::cout << "Left current " << leftMotors.get_current_draw_all()[0] << " " << leftMotors.get_current_draw_all()[1] << " " << leftMotors.get_current_draw_all()[2] << std::endl;
    std::cout << "Right current " << rightMotors.get_current_draw_all()[0] << " " << rightMotors.get_current_draw_all()[1] << " " << rightMotors.get_current_draw_all()[2] << std::endl;


    std::cout << "Left temp " << leftMotors.get_temperature_all()[0] << " " << leftMotors.get_temperature_all()[1] << " " << leftMotors.get_temperature_all()[2] << std::endl;
    std::cout << "Right temp " << rightMotors.get_temperature_all()[0] << " " << rightMotors.get_temperature_all()[1] << " " << rightMotors.get_temperature_all()[2] << std::endl;
    */
    
    base.tank(leftVelocity, rightVelocity); // Configures the base to have tank control (one joystick direction controls one side of the robot). Simply uses the left and right
                                                        // joysticks.
}