// The LemLib chassis library used for chassis movement. Relevant PROS library for controller inputs, optical outputs, motor control, and pneumatic control (embeded in various other 
// includes). We are using the Using standard C++ array library to easily return information and the standard C++ math library to do calculations.
#include <array>
#include <cmath>
#include <fstream>

#include "pros/misc.h"
#include "pros/rtos.hpp"

#include "lemlib/chassis/chassis.hpp"

#include "my_includes/global_var.h"
#include "my_includes/states.h"
#include "my_includes/MyPID.h"
#include "my_includes/ports.h"



// Top hook positions: 1st = 2.0 / 6.0, 2nd = 17.0 / 6.0, 3rd = 33.0 / 6.0
// Neural hook position: 1st = 0.0, 2nd = 15.0 / 6.0, 3rd = 31.0 / 6.0
// The hooks should be at the same positions every 46.0 / 6.0 rotations or 7.6666... rotations (Numbers are based on chains over knobs on a sprocket)

// 16, 16, 17

// Function to find potential hook positions where they are in a "top position" (i.e. where hooks are at their peak)
std::array<float, 3> nextTopHook() {
    float gearRatio = 48.0 / 36.0;

    float fullRotations = std::floor(intake.get_position() * 6.0 / 49.0 * gearRatio); // Used to find how many full rotations have been completed

    if ((intake.get_position() * 6.0 / 49.0 - fullRotations) * gearRatio >= (36.0 / 49.0)) // Only relevant to bringing a hook to its peak. If the hooks have not completed a full rotation yet the
        fullRotations += 1;                                                                // the third hook has already passed its peak we have to look at the next rotation otherwise the intake
                                                                                           // goes backwards. When there is a remainder of 33 / 46 or 0.71739... (percentage of the rotation travelled 
                                                                                           // after the third hook) we know this is the case.

    float firstHookPosition = ((2.0 / 6.0) + (49.0 / 6.0) * fullRotations) * gearRatio; // Math for calculating where the hooks could be. The logic is explained above.
    float secondHookPosition = ((18.0 / 6.0) + (49.0 / 6.0) * fullRotations) * gearRatio;
    float thirdHookPosition = ((35.0 / 6.0) + (49.0 / 6.0) * fullRotations) * gearRatio;

    return {firstHookPosition, secondHookPosition, thirdHookPosition}; // Returns an array of the potential positions for use elsewhere in the code
}

// Function to find potential hook positions where they are in a "neutral position" (i.e. where hooks are relatively parallel to the ground to avoid the first rung of the ladder). All of
// the logic is the same as the logic used for finding potential top hook
std::array<float, 3> nextNeutralHook() {
    float gearRatio = 48.0 / 36.0;

    float fullRotations = std::floor(intake.get_position() * 6.0 / 49.0 * gearRatio); 

    float firstHookPosition = (49.0 / 6.0) * fullRotations * gearRatio;
    float secondHookPosition = ((16.0 / 6.0) + (49.0 / 6.0) * fullRotations) * gearRatio;
    float thirdHookPosition = ((33.0 / 6.0) + (49.0 / 6.0) * fullRotations) * gearRatio;

    return {firstHookPosition, secondHookPosition, thirdHookPosition};
}



// For clarity when I am writing the code
enum DesiredHookPositions {
    TOP,
    NEUTRAL
};



float findClosestHook(DesiredHookPositions desiredHookPosition) {
    std::array<float, 3> potentialPositions; // An array for storing all potentially viable hook positions

    std::cout << intake.get_position() << std::endl;


    if (desiredHookPosition == TOP) {
        potentialPositions = nextTopHook();

        std::cout << potentialPositions[0] << ", " << potentialPositions[1] << ", " << potentialPositions[2] << std::endl;

        // Runs through every item of the potential positions array
        for (int i = 0; i < potentialPositions.size(); i++) {
            std::cout << potentialPositions[i] << std::endl;

            if (potentialPositions[i] - intake.get_position() > 0) // If the value is negative we know that the hook associated with the potential position has already passed the 
                return potentialPositions[i];                          // optical sensor and thus cannot hold the ring. As the potential positions array is arranged from smallest to
                                                                       // largest, we can confidently assume that the first valid position matches to the correct hook.
        }
        
    }
    else if (desiredHookPosition == NEUTRAL) {
        potentialPositions = nextNeutralHook();

        std::cout << potentialPositions[0] << ", " << potentialPositions[1] << ", " << potentialPositions[2] << std::endl;
        
        for (int i = 0; i < potentialPositions.size(); i++) {
            std::cout << potentialPositions[i] << std::endl;

            if (potentialPositions[i] - intake.get_position() <= 0) // If the value is positive we know that the hook associated with the potential position needs to move forward to   
                return potentialPositions[i];                           // reach a "neutral position". As we want the intake to move backwards to avoid catching on mogos, we can 
                                                                        // immediately discard it. The equal check is to catch when the intake has not moved yet we accidently call it to 
                                                                        // reach a neutral position to avoid breaking the programming. As stated earlier the potential positions array is                                                                         // 
                                                                        // arragned from smallest to largest, however the logic this time is that the last valid position should match to
                                                                        // the correct hook as it would be the closest to the current position without having the intake move forwards.
        }
    }
    
    return 0;
}

void moveIntakeToDesiredPosition(DesiredHookPositions desiredHookPosition) {
    MyPID intakePID(200, 0, 0, 0, 0, 0, 400);

    float desiredPosition = findClosestHook(desiredHookPosition); // Find out where the intake needs to rotate to in order to properly position a hook.

    float error;

    intake.move_absolute(desiredPosition, 600); // Starts rotating the intake towards the position.

    // While error is greater than 0.0125 keep waiting 5 milliseconds to give the move_absolute function time.
    while (std::abs(desiredPosition - intake.get_position()) > 0.1) {
        error = (desiredPosition - intake.get_position()) * 360;

        intakePID.update(error);

        intake.move_velocity(intakePID.getVelocity());

        // std::cout << "Velocity is " << intake.get_actual_velocity() << std::endl;
        pros::delay(5);
    }

    std::cout << intake.get_position() << std::endl;

    intake.brake();

    if (desiredHookPosition == TOP) // If we want to throw a ring off (color sort) then we want a delay to ensure the ring's momentum properly carries it off the hooks
        pros::delay(500);
}



bool checkForColor(bool opposite) {
    if (((global::autonSelected == states::autonStates::RedPositiveCorner || global::autonSelected == states::autonStates::RedNegativeCorner || global::autonSelected == states::autonStates::Skills) && !opposite) || 
        ((global::autonSelected == states::autonStates::BluePositiveCorner || global::autonSelected == states::autonStates::BlueNegativeCorner) && opposite))
            return optical.get_hue() < 18; // Check for red rings which have an approximate hue range of 15-18
    else if (((global::autonSelected == states::autonStates::RedPositiveCorner || global::autonSelected == states::autonStates::RedNegativeCorner || global::autonSelected == states::autonStates::Skills) && opposite) || 
             ((global::autonSelected == states::autonStates::BluePositiveCorner || global::autonSelected == states::autonStates::BlueNegativeCorner) && !opposite))
            return optical.get_hue() > 167; // Check for blue rings which have an approximate hue range of 167-

    return false;
}



void handleIntake() {
    int intakeSpeed = 0;

    bool timerRunning = false;
    int32_t jamStartTime = 0;

    while (true) {
        // std::cout << "Hue is " << optical.get_hue() << std::endl;
        // std::cout << "intake state is " << global::intakeState << std::endl;

        // If the driver has not overriden the color sort, look for opposing rings to throw off at the top.
        if (checkForColor(!global::flipColorSort) && !global::overrideColorSort) {
            moveIntakeToDesiredPosition(TOP); // Moves a hook to its top position

            std::cout << "Here hello" << std::endl;
        }

        // Same logic as the color sorter except for moving a hook to its neutral position
        if (global::findNextDown) { 
            intake.brake();

            moveIntakeToDesiredPosition(NEUTRAL);

            global::findNextDown = false;
        }



        if (global::intakeState == states::intakeStates::StoreRing && checkForColor(global::flipColorSort))
            global::intakeState = states::intakeStates::Resting;

        if (global::intakeState == states::intakeStates::Arm && intake.get_torque() > 0.34) {
            std::cout << "Hello world ring in LB" << std::endl;
            global::intakeState = states::intakeStates::Resting;
        }



        if (intake.get_torque() > 0.34 && !timerRunning && global::intakeState != states::intakeStates::Arm) {
            timerRunning = true;
            jamStartTime = pros::millis();
        } else if (intake.get_torque() < 0.34 && timerRunning || global::intakeState == states::intakeStates::Arm)
            timerRunning = false;

        if (timerRunning && pros::millis() - jamStartTime > 250) {
            intake.move_velocity(-600);
            pros::delay(250);
            intake.move_velocity(intakeSpeed);

            timerRunning = false;
        }



        if (global::intakeState == states::intakeStates::Mogo || global::intakeState == states::intakeStates::StoreRing 
            || global::intakeState == states::intakeStates::Arm) // Sets the intake to 600 rpm, running directly to the mogo
            intakeSpeed = 600;
        else if (global::intakeState == states::intakeStates::Resting) // Stops the intake
            intakeSpeed = 0;
            // std::cout << intakeSpeed << std::endl;
        else if (global::intakeState == states::intakeStates::Reverse) // Reverses the intake at 600 rpm
            intakeSpeed = -600;
        


        intake.move_velocity(intakeSpeed); 
        
        // if (intake.get_actual_velocity() != 0)
            // std::cout << "Velocity is " << intake.get_actual_velocity() << " target velocity is " << intake.get_target_velocity() << std::endl;



        pros::delay(5); // A delay to ensure all processes run smoothly
    }
}




void handleArm() {
    // PID related variables
    float targetArmPosition = 0;
    float currentArmPosition = 0;

    float armVelocity = 0;

    float error = 0; // slew is 30

    bool inMotion = false; // Whether or not the arm is still moving

    MyPID armPID(0.65, 0.0, 0.6, 0.0, 5.0, 10, 0); // An instance of the MyPID class to calculate the outputs



    bool removedBuffer = true; // Whether or not we have completed a certain motion


    
    while (true) {
        // The variety of positions the arm should move to in order to score or do the associated action. "Prime" refers to the position in which we can store a ring in the arm.        
        if (global::armState == states::armStates::Resting)
            targetArmPosition = 0;
        else if (global::armState == states::armStates::PrimeOne)
            targetArmPosition = 100;
        else if (global::armState == states::armStates::PrimeTwo)
            targetArmPosition = 130;
        else if (global::armState == states::armStates::WallStake)
            targetArmPosition = 350;
        else if (global::armState == states::armStates::AllianceStake)
            targetArmPosition = 450;
        else if (global::armState == states::armStates::TipMogo)
            targetArmPosition = 500; // 500



        currentArmPosition = (arm.get_position_all()[0] + arm.get_position_all()[1]) / 2 - 30; // Taking the average of boths motors to hopefully filter out error. I want to offset the position
                                                                                               // by 30 degrees to avoid having the resting position crash into the metal all the time.

        error = targetArmPosition - currentArmPosition; // Calculates the error for the PID

        armPID.update(error); // Updates all the values of the PID to get the next output



        inMotion = !armPID.earlyExit(); // Check to see if the PID is within its exit range so it is not stuck on one motion

        if (inMotion) {
            // If we are not within the exit range move the arm at the velocity of the PID output
            armVelocity = armPID.getVelocity();

            arm.move_velocity(armVelocity);
        } else {
            arm.brake(); // Stops the arm



            armPID.reset(); // Resets the PID for the next motion



            // Basically a check for whether we have completed the motion and can move onto the next item in queue
            if (!removedBuffer) {
                global::armStatesQueue.erase(global::armStatesQueue.begin()); // Deletes the item from the queue
                removedBuffer = true;                                                  // Ensures that we do not delete the entire queue without completing all the actions
            }



            // Basically checks for if we have a queue
            if (global::armStatesQueue.size() != 0) {
                global::armState = global::armStatesQueue.begin()[0]; // If we do have a queue we move the arm to the first/next position in queue
                removedBuffer = false;                                // Reset the buffer variable so we know we have not completed the motion
            }
        }



        pros::delay(100);
    }
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
    } else {
        // When the chassis is reversed
        leftVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * -1); // Invert the controllers and swap the sticks
        rightVelocity = driveCurve.curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * -1); // (i.e., left now controls rght and vice versa)
    }

    if (controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) != 0)
        std::cout << "Right " << controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) << std::endl;

    if (controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) != 0)
        std::cout << "Left " << controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) << std::endl;


    /*
    std::cout << "Left current " << leftMotors.get_current_draw_all()[0] << " " << leftMotors.get_current_draw_all()[1] << " " << leftMotors.get_current_draw_all()[2] << std::endl;
    std::cout << "Right current " << rightMotors.get_current_draw_all()[0] << " " << rightMotors.get_current_draw_all()[1] << " " << rightMotors.get_current_draw_all()[2] << std::endl;


    std::cout << "Left temp " << leftMotors.get_temperature_all()[0] << " " << leftMotors.get_temperature_all()[1] << " " << leftMotors.get_temperature_all()[2] << std::endl;
    std::cout << "Right temp " << rightMotors.get_temperature_all()[0] << " " << rightMotors.get_temperature_all()[1] << " " << rightMotors.get_temperature_all()[2] << std::endl;
    */
    
    base.tank(leftVelocity, rightVelocity); // Configures the base to have tank control (one joystick direction controls one side of the robot). Simply uses the left and right
                                                        // joysticks.
}



void storeCoordinates() {
    std::string myText;

    std::ofstream matchCoordiantes("matchCoordinates.txt", std::ios_base::app);

    matchCoordiantes << "Start of program " << std::endl;

    matchCoordiantes.close();

    bool newAuton = true;
    bool newOpControl = true;

    /*
    while (getline (matchCoordiantes, myText)) {
        // Output the text from the file
        std::cout << myText;
    }
    */

    while (true) {
        while (global::runningAuton) {
            std::ofstream matchCoordiantes("matchCoordinates.txt", std::ios_base::app);

            if (newAuton) {
                matchCoordiantes << "Start of auton" << std::endl;
                newAuton = false;
            }

            matchCoordiantes << "x: " << base.getPose().x << ", y: " << base.getPose().y << ", theta: " << base.getPose().theta << std::endl;


            matchCoordiantes.close();



            pros::delay(5);
        }

        newAuton = true;



        while (global::runningOpControl) {
            std::ofstream matchCoordiantes("matchCoordinates.txt", std::ios_base::app);

            if (newOpControl) {
                matchCoordiantes << "Start of op control" << std::endl;
                newOpControl = false;
            }

            matchCoordiantes << "x: " << base.getPose().x << ", y: " << base.getPose().y << ", theta: " << base.getPose().theta << std::endl;

            matchCoordiantes.close();

            pros::delay(5);
        }



        newOpControl = true;



        pros::delay(5);
    }

    matchCoordiantes << "End of program, goodbye" << std::endl;
}