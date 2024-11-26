// Lemlib chassis library used for chassis movement. Relevant PROS library for controller inputs, optical outputs, motor control, 
// and pneumatic control (embeded in various other includes).
#include "lemlib/chassis/chassis.hpp"

#include "global_var.h"
#include "ports.h"
#include "pros/misc.h"
#include "states.h"



bool overrideColorSort = false;
bool flipColorSort = false;

void updateIntakeStates() {
    // All the code for controllering the intake state
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) // When the L1 button is pressed
    {
        // Toggle the intake to the mogo or resting state with the same button
        if (intakeState == intakeResting || intakeState == intakeReverse)
            intakeState = intakeMogo;
        else
            intakeState = intakeResting;
    }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // When the R2 button is pressed set the intake to the reverse state
        intakeState = intakeReverse;                                       
    else if (intakeState != intakeMogo) // If the last state was reverse and the R2 button was let go, set the intake to its resting state
        intakeState = intakeResting;

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) // When the down button is pressed toggle the color sorter override
        flipColorSort = !flipColorSort;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) // When the up button is pressed flip the color the color sorter looks for
        overrideColorSort = !overrideColorSort;
    
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) // When the up button is pressed flip the color the color sorter looks for
    {
        storeRing = true;
        std::cout << "Hello there" << std::endl;
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) // When the B button is pressed find the next valid "down" position
        findNextDown = true;
}

bool checkForColor(bool opposite) {
    if (autonSelected == redPositiveCorner || autonSelected == redNegativeCorner || autonSelected == skills) {
        // If apart of the red alliance or selected the skills autonomus routine
        if (opposite) // Check for blue rings which have an approximate hue range of 167-210
            return optical.get_hue() > 167;
        else // Check for red rings which have an approximate hue range of 15-18
            return optical.get_hue() < 18;
    }
    else if (autonSelected == bluePositiveCorner || autonSelected == blueNegativeCorner)  {
        // Same logic as above but vice versa
        if (opposite)
            return optical.get_hue() < 167;
        else
            return optical.get_hue() > 18;
    }

    return false;
}

void handleIntake() {
    double nextTopPosition = 0.32; // First position where hooks are at the top (in rotations)
    double nextDownPosition = 0; // First position where hooks are right before the top (in rotations)

    /*
    double highestHue = 18;
    double lowestHue = 17;
    */

    while (true)
    {
        /* Debug loggers
        if (optical.get_hue() > highestHue)
            highestHue = optical.get_hue();
        else if (optical.get_hue() < lowestHue && optical.get_hue() != 0)
            lowestHue = optical.get_hue();

        std::cout << "Color is " << optical.get_hue() << ", time is " << pros::millis() << std::endl; // For figuring out color
        std::cout << "Lowest hue is " << lowestHue << std::endl;
        std::cout << "Highest hue is " << highestHue << std::endl;
        */

        

        if (checkForColor(!flipColorSort) && !overrideColorSort) // Check for the opposing alliance ring
        {
            intakeHook.move_velocity(0); // Stops the intake so that all necessary calcuations can occur

            while (nextTopPosition < intakeHook.get_position()) // Until the next valid position is found
                nextTopPosition += (15.0 + 1.0 / 3.0) / 6.0;                  // There is a hook at the top every 23/6 (~3.8333) rotations

            intakeHook.move_absolute(nextTopPosition, 600); // Move the hooks to the top of the intake

            while (nextTopPosition - intakeHook.get_position() >= 0.0125) // Delays the code until the intake is within 0.025 revolutions from its desired position so that the move_absolute 
                pros::delay(5);                            // function has time to run

            std::cout << nextTopPosition - intakeHook.get_position() << std::endl; 

            pros::delay(500); // Creates a half second delay to ensure he ring is flung off
        }



        if (findNextDown) // Same logic as the color sorter except for the zero point
        {
            intakeHook.move_velocity(0); // Stops the intake so that all necessary calcuations can occur

            while (nextDownPosition < intakeHook.get_position())
                    nextDownPosition += (15.0 + 1.0 / 3.0) / 6.0;

            nextDownPosition -= (15.0 + 1.0 / 3.0) / 6.0; // Ensures that the hooks will move backwards to avoid getting caught on rings scored on the mogo

            intakeHook.move_absolute(nextDownPosition, 600);

            while (nextDownPosition - intakeHook.get_position() <= -0.0125)
                pros::delay(5); 
            
            findNextDown = false;
        }
        


        if (storeRing) {
            std::cout << "Hello there" << std::endl;

            intakePre.move_velocity(600);
            intakeHook.move_velocity(459);

            while (!checkForColor(false)) {
                pros::delay(5);
            }

            intakePre.move_velocity(0);
            intakeHook.move_velocity(0);
            storeRing = false;
        }


        if (intakeState == intakeMogo) // Sets the intake to 600 rpm, running directly to the mogo
            intakeSpeed = 600;
        else if (intakeState == intakeResting) // Stops the intake
            intakeSpeed = 0;
        else if (intakeState == intakeReverse) // Reverses the intake at 600 rpm
            intakeSpeed = -600;

        if (!overrideIntakeState)
        {
            intakePre.move_velocity(intakeSpeed);              // Runs the first stage intake at whatever rpm was set
            intakeHook.move_velocity(intakeSpeed * 459 / 600); // Runs the second stage intake at a slightly slower speed to ensure rings are placed on the mogo properly
        }

        // 459 - 438 = 21 = 11 = 448

        pros::delay(5); // A delay to ensure all processes run smoothly
    }
}



void handlePneumatics() {
    // When the R1 button is pressed toggle the mogo clamp
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
    {
        if (mogoClamp.is_extended())
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

    if (!reverse) // Regular chassis controls
    {
        leftVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        rightVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
    }
    else // When the chassis is reversed
    {
        leftVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * -1); // Invert the controllers and swap the sticks
        rightVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * -1); // (i.e., left now controls rght and vice versa)
    }


    std::cout << "Left current " << leftMotors.get_current_draw_all()[0] << " " << leftMotors.get_current_draw_all()[1] << " " << leftMotors.get_current_draw_all()[2] << std::endl;
    std::cout << "Right current " << rightMotors.get_current_draw_all()[0] << " " << rightMotors.get_current_draw_all()[1] << " " << rightMotors.get_current_draw_all()[2] << std::endl;


    std::cout << "Left temp " << leftMotors.get_temperature_all()[0] << " " << leftMotors.get_temperature_all()[1] << " " << leftMotors.get_temperature_all()[2] << std::endl;
    std::cout << "Right temp " << rightMotors.get_temperature_all()[0] << " " << rightMotors.get_temperature_all()[1] << " " << rightMotors.get_temperature_all()[2] << std::endl;

    
    base.tank(leftVelocity, rightVelocity); // Configures the base to have tank control (one joystick direction controls one side of the robot). Simply uses the left and right
                                                        // joysticks.
}