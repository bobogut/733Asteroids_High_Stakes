// The LemLib chassis library used for chassis movement. Relevant PROS library for controller inputs, optical outputs, motor control, and pneumatic control (embeded in various other 
// includes). We are using the Using standard C++ array library to easily return information and the standard C++ math library to do calculations.
#include <array>
#include <cmath>
#include <fstream>

#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"

#include "lemlib/chassis/chassis.hpp"

#include "my_includes/global_var.h"
#include "my_includes/states.h"
#include "my_includes/MyPID.h"
#include "my_includes/ports.h"



// Function to find potential hook positions where they are in a "top position" (i.e. where hooks are at their peak)
std::array<int, 3> nextTopHooks() {
    int fullRotations = intakeSecondStage.get_position() / 2385; // Used to find how many full rotations have been completed, one full rotation is 2385 degrees

    if ((intakeSecondStage.get_position() - 2385   * fullRotations) >= 1755) // Only relevant to bringing a hook to its peak. If the hooks have not completed a full rotation yet the
        fullRotations += 1;                                                  // the third hook has already passed or is at its peak we have to look at the next rotation otherwise the 
                                                                             // intake goes backwards. When there is a remainder of 1755 degrees we know this is the case.
    
    int firstHookPosition = 180 + 2385 * fullRotations; // Math for calculating where the hooks could be. The logic is explained above.
    int secondHookPosition = 990 + 2385 * fullRotations;
    int thirdHookPosition = 1755 + 2385 * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition}; // Returns an array of the potential positions for use elsewhere in the code
}

// Function to find potential hook positions where they are in a "neutral position" (i.e. where hooks are relatively parallel to the ground to avoid the first rung of the ladder). All of
// the logic is the same as the logic used for finding potential top hook
std::array<int, 3> nextNeutralHooks() {
    int fullRotations = intakeSecondStage.get_position() / 2385;

    int firstHookPosition = 2385 * fullRotations;
    int secondHookPosition = 810 + 2385 * fullRotations;
    int thirdHookPosition = 1575 + 2385 * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition};
}



std::array<int, 3> nextArmScoreHooks() {
    int fullRotations = intakeSecondStage.get_position() / 2385; 

    if ((intakeSecondStage.get_position() - 2385   * fullRotations) >= 1755)
        fullRotations += 1;  

    int firstHookPosition = 230 + 2385 * fullRotations;
    int secondHookPosition = 1040 + 2385 * fullRotations;
    int thirdHookPosition = 1805 + 2385 * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition};
}


// For clarity when I am writing the code
enum DesiredHookPositions {
    Top,
    Neutral,
    Arm
};



float findClosestHook(DesiredHookPositions desiredHookPosition) {
    std::array<int, 3> potentialPositions; // An array for storing all potentially viable hook positions



    if (desiredHookPosition == Top) {
        potentialPositions = nextTopHooks();

        // std::cout << "Hook at " << intakeSecondStage.get_position() << std::endl;

        // Runs through every item of the potential positions array
        for (int i = 0; i < potentialPositions.size(); i++) {
            std::cout << "Hook " << i + 1 << " is " << potentialPositions[i] << std::endl;

            if (potentialPositions[i] - intakeSecondStage.get_position() > 0) // If the value is negative we know that the hook associated with the potential position has already passed the 
                return potentialPositions[i];                                 // optical sensor and thus cannot hold the ring. As the potential positions array is arranged from smallest to
                                                                              // largest, we can confidently assume that the first valid position matches to the correct hook.
            std::cout << "Hook " << i + 1 << " invalid " << potentialPositions[i] << std::endl;

        }
        
    } else if (desiredHookPosition == Neutral) {
        potentialPositions = nextNeutralHooks();

        for (int i = 0; i < potentialPositions.size(); i++) {
            // std::cout << potentialPositions[i] << std::endl;

            if (potentialPositions[i] - intakeSecondStage.get_position() <= 0) // If the value is positive we know that the hook associated with the potential position needs to move forward to   
                return potentialPositions[i];                                  // reach a "neutral position". As we want the intake to move backwards to avoid catching on mogos, we can 
                                                                               // immediately discard it. The equal check is to catch when the intake has not moved yet we accidently call it to 
                                                                               // reach a neutral position to avoid breaking the programming. As stated earlier the potential positions array is                                                                         // 
                                                                               // arragned from smallest to largest, however the logic this time is that the last valid position should match to
                                                                               // the correct hook as it would be the closest to the current position without having the intake move forwards.
        }
    } else if (desiredHookPosition == Arm) {
        potentialPositions = nextArmScoreHooks();

        // std::cout << potentialPositions[0] << ", " << potentialPositions[1] << ", " << potentialPositions[2] << std::endl;
        
        for (int i = 0; i < potentialPositions.size(); i++) {
            // std::cout << potentialPositions[i] << std::endl;

            std::cout << "Hook " << i + 1 << " is " << potentialPositions[i] << std::endl;

            if (potentialPositions[i] - intakeSecondStage.get_position() > 0) // Same logic as the top hook position
                return potentialPositions[i];     

            std::cout << "Hook " << i + 1 << " invalid " << potentialPositions[i] << std::endl;
        }
    }
    
    return intakeSecondStage.get_position();
}

void moveIntakeToDesiredPosition(DesiredHookPositions desiredHookPosition) {
    MyPID intakePID(0.002, 0, 0.3, 0, 5, 20, 1000, 0);

    float desiredPosition = findClosestHook(desiredHookPosition); // Find out where the intake needs to rotate to in order to properly position a hook.

    float error;
    float intakeVelocity;

    intakePID.update(error);

    while (!intakePID.earlyExit()) {
        error = desiredPosition - intakeSecondStage.get_position();

        intakePID.update(error);

        if (intakeSecondStage.get_position() > desiredPosition && (desiredHookPosition == Top || desiredHookPosition == Arm))
            break;

        intakeVelocity = intakePID.getVelocity();



        intakeSecondStage.move_velocity(intakeVelocity);

        // std::cout << intakeSecondStage.get_position() << " " << intakeSecondStage.get_actual_velocity() << " " << intakePID.getTime() << " " << desiredPosition << std::endl;



        pros::delay(5);
    }
}



bool checkForColor(bool opposite) {
    if (((global::autonSelected == states::autonStates::RedPositiveCorner || global::autonSelected == states::autonStates::RedNegativeCorner || global::autonSelected == states::autonStates::Skills) && !opposite) || 
        ((global::autonSelected == states::autonStates::BluePositiveCorner || global::autonSelected == states::autonStates::BlueNegativeCorner) && opposite))
            return optical.get_hue() < 20; // Check for red rings which have an approximate hue range of 15-18
    else if (((global::autonSelected == states::autonStates::RedPositiveCorner || global::autonSelected == states::autonStates::RedNegativeCorner || global::autonSelected == states::autonStates::Skills) && opposite) || 
             ((global::autonSelected == states::autonStates::BluePositiveCorner || global::autonSelected == states::autonStates::BlueNegativeCorner) && !opposite))
            return optical.get_hue() > 200; // Check for blue rings which have an approximate hue range of 220-
    else if (global::autonSelected == states::autonStates::None)
        return false;

    return false;
}



bool inSortRange(int targetPosition) {
    return (targetPosition - intakeSecondStage.get_position()) <= 0;
}



void handleIntake() {
    std::cout << "init pos is " << intakeSecondStage.get_position() << std::endl;


    int intakeSpeed = 0;

    bool timerRunning = false;
    int32_t jamStartTime = 0;

    bool sortRing = false;
    int targetPosition = 0;

    while (true) {
        // If the driver has not overriden the color sort, look for opposing rings to throw off at the top.

        if (checkForColor(!global::flipColorSort) && !global::overrideColorSort) {
            if (!sortRing)
                targetPosition = findClosestHook(Top);

            sortRing = true;
        } 


        if (global::intakeState == states::intakeStates::Mogo && global::armState == states::armStates::PrimeOne && checkForColor(global::flipColorSort)) {
            moveIntakeToDesiredPosition(Arm);
        
            global::intakeState = states::intakeStates::Resting;
        }



        // If the robot knows it has it an enemy ring and is within a few degrees of the position
        if (sortRing && inSortRange(targetPosition)) {
            intakeSecondStage.set_brake_mode(pros::v5::MotorBrake::brake); // Stop the untake for 500 ms, brake mode is set to brake to ensure a quick stop.
            intakeSecondStage.brake();                                          // Stopping the hook should allow the ring's momentum to carry it over the mogo.
            pros::delay(500);

            intakeSecondStage.move_velocity(-600);

            pros::delay(100);
            
            intakeSecondStage.set_brake_mode(pros::v5::MotorBrake::coast); // Brake mode is set back to coast to avoid motor burn out

            sortRing = false; // Acknowledge that the ring has been sorted
        } 



        // Same logic as the color sorter except for moving a hook to its neutral position
        if (global::intakeState == states::intakeStates::FindNextDown) { 
            moveIntakeToDesiredPosition(Neutral);

            global::intakeState = states::intakeStates::Resting;
        }




        // If the torque caps out and the timer is not running, start the timer
        if (intakeSecondStage.get_torque() >= 0.34 && !timerRunning) {
            timerRunning = true;
            jamStartTime = pros::millis();
        } else if (intakeSecondStage.get_torque() < 0.34 && timerRunning )
            timerRunning = false; // If the torque is  no longer capped out and the timer is running, stop the timer

        // If the timer is running and passed
        if (timerRunning && pros::millis() - jamStartTime >= 250 && global::armState != states::armStates::PrimeOne) {
            intakeSecondStage.move_velocity(-600); // Reverse the second stage of the intake for 750 ms, this stops the hooks from interfering with the ring
            pros::delay(500);
            intakeSecondStage.move_velocity(intakeSpeed); // Start moving the intake the right direction

            timerRunning = false; // Disable the timer
        } else if (timerRunning && pros::millis() - jamStartTime >= 500 && global::armState == states::armStates::PrimeOne)
            global::intakeState = states::intakeStates::Resting;



        if (global::intakeState == states::intakeStates::Mogo || global::intakeState == states::intakeStates::StoreRing 
            || global::intakeState == states::intakeStates::FirstStage)
            intakeSpeed = 600;  // Sets the intake to 600 rpm, running directly to the mogo
        else if (global::intakeState == states::intakeStates::Resting) // Stops the intake
            intakeSpeed = 0;
        else if (global::intakeState == states::intakeStates::Reverse) // Reverses the intake at 600 rpm
            intakeSpeed = -600;

        intakeFirstStage.move_velocity(intakeSpeed);

        if (global::armState == states::armStates::WallStake)
            intakeSecondStage.move_velocity(-intakeSpeed * 150 / 600);
        else if (global::intakeState != states::intakeStates::FirstStage)
            intakeSecondStage.move_velocity(intakeSpeed);
        else
            intakeSecondStage.move_velocity(0);



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

    MyPID armPID(0.75, 0.0, 0.1, 0.0, 1, 500, 2000, 0); // An instance of the MyPID class to calculate the outputs



    bool removedBuffer = true; // Whether or not we have completed a certain motion
    bool freeze = true;


    
    while (true) {
        // The variety of positions the arm should move to in order to score or do the associated action. "Prime" refers to the position in which we can store a ring in the arm.        
        if (global::armState == states::armStates::Resting)
            targetArmPosition = 0;
        else if (global::armState == states::armStates::PrimeOne)
            targetArmPosition = 90; // Numbers from testing
        else if (global::armState == states::armStates::PrimeTwo)
            targetArmPosition = 180;
        else if (global::armState == states::armStates::WallStake)
            targetArmPosition = 530;
        else if (global::armState == states::armStates::AllianceStake)
            targetArmPosition = 730;
        else if (global::armState == states::armStates::TipMogo)
            targetArmPosition = 830;



        currentArmPosition = arm.get_position();

        error = targetArmPosition - currentArmPosition; // Calculates the error for the PID

        armPID.update(error); // Updates all the values of the PID to get the next output



        inMotion = !armPID.earlyExit(); // Check to see if the PID is within its exit range so it is not stuck on one motion

        // If we are not within the exit range
        if (inMotion && !freeze) {
            armVelocity = armPID.getVelocity(); // Get the velocity

            arm.move_velocity(armVelocity); // Move the arm at the appropiate speed
        } else {
            arm.brake(); // Stops the arm

            armPID.reset(); // Resets the PID for the next motion



            // Basically a check for whether we have completed the motion and can move onto the next item in queue
            if (!removedBuffer) {
                if (global::armStatesQueue.size() != 0)
                    global::armStatesQueue.erase(global::armStatesQueue.begin()); // Deletes the item from the queue
                removedBuffer = true;                                                      // Ensures that we do not delete the entire queue without completing all the actions
            }

            // Basically checks for if we have a queue
            if (global::armStatesQueue.size() != 0 && !(global::armState == states::armStates::WallStake 
                && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
                global::armState = global::armStatesQueue.begin()[0]; // If we do have a queue we move the arm to the first/next position in queue
                removedBuffer = false;                                // Reset the buffer variable so we know we have not completed the motion
                freeze = false;
            } else
                freeze = true; // Used to prevent a motion from starting again without registering a new state
        }



        pros::delay(5);
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