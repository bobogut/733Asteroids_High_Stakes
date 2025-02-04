// The LemLib chassis library used for chassis movement. Relevant PROS library for controller inputs, optical outputs, motor control, and pneumatic control (embeded in various other 
// includes). We are using the Using standard C++ array library to easily return information and the standard C++ math library to do calculations.
#include <array>
#include <cmath>
#include <fstream>

#include "MyPID.h"

#include "lemlib/chassis/chassis.hpp"

#include "global_var.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

#include "states.h"
#include "ports.h"



// Top hook positions: 1st = 2.0 / 6.0, 2nd = 17.0 / 6.0, 3rd = 33.0 / 6.0
// Neural hook position: 1st = 0.0, 2nd = 15.0 / 6.0, 3rd = 31.0 / 6.0
// The hooks should be at the same positions every 46.0 / 6.0 rotations or 7.6666... rotations (Numbers are based on chains over knobs on a sprocket)

// Function to find potential hook positions where they are in a "top position" (i.e. where hooks are at their peak)
std::array<double, 3> nextTopHook() {
    double fullRotations = std::floor(intake.get_position() * 6.0 / 46.0); // Used to find how many full rotations have been completed

    if (intake.get_position() * 6.0 / 48.0 - fullRotations >= (33.0 / 48.0)) // Only relevant to bringing a hook to its peak. If the hooks have not completed a full rotation yet the
        fullRotations += 1;                                                      // the third hook has already passed its peak we have to look at the next rotation otherwise the intake
                                                                                 // goes backwards. When there is a remainder of 33 / 46 or 0.71739... (percentage of the rotation travelled 
                                                                                 // after the third hook) we know this is the case.

    double firstHookPosition = (2.0 / 6.0) + (48.0 / 6.0) * fullRotations; // Math for calculating where the hooks could be. The logic is explained above.
    double secondHookPosition = (17.0 / 6.0) + (48.0 / 6.0) * fullRotations;
    double thirdHookPosition = (33.0 / 6.0) + (48.0 / 6.0) * fullRotations;

    return {firstHookPosition, secondHookPosition, thirdHookPosition}; // Returns an array of the potential positions for use elsewhere in the code
}

// Function to find potential hook positions where they are in a "neutral position" (i.e. where hooks are relatively parallel to the ground to avoid the first rung of the ladder). All of
// the logic is the same as the logic used for finding potential top hook
std::array<double, 3> nextNeutralHook() {
    double fullRotations = std::floor(intake.get_position() * 6.0 / 46.0); 

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
    std::array<double, 3> potentialPositions; // An array for storing all potentially viable hook positions

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

void moveIntakeToDesiredPosition(DesiredHookPositions desiredHookPosition, double desiredPosition) {
    // double desiredPosition = findClosestHook(desiredHookPosition); // Find out where the intake needs to rotate to in order to properly position a hook.

    intake.move_absolute(desiredPosition, 600); // Starts rotating the intake towards the position.

    while (std::abs(desiredPosition - intake.get_position()) > 1.0 / 12.0) // While error is greater than 0.0125 keep waiting 5 milliseconds to give the move_absolute function time.
        pros::delay(5);

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
    /*
    Variables for debugging

    double highestHue = 18;
    double lowestHue = 17;
    */
    
    int intakeSpeed;
    int modifier = 600;

    double intakePosition = 0;

    bool timerRunning = false;
    int32_t jamStartTime = 0;

    while (true)
    {
        // std::cout << "global::intakeState is " <<global::intakeState << std::endl;
        /* 
        Loggers for debugging

        if (optical.get_hue() > highestHue)
            highestHue = optical.get_hue();
        else if (optical.get_hue() < lowestHue && optical.get_hue() != 0)
            lowestHue = optical.get_hue();

        std::cout << "Lowest hue is " << lowestHue << std::endl;
        std::cout << "Highest hue is " << highestHue << std::endl;
        std::cout << "Color is " << optical.get_hue() << ", time is " << pros::millis() << std::endl; // For figuring out color
        */

        // std::cout << ((global::autonSelected == states::autonStates::RedPositiveCorner || global::autonSelected == states::autonStates::RedNegativeCorner || global::autonSelected == states::autonStates::Skills) && !flipColorSort) << std::endl;
        /*
        // If the driver has not overriden the color sort, look for opposing rings to throw off at the top.
        if (checkForColor(!global::flipColorSort) && !global::overrideColorSort) {
            std::cout << "Here" << std::endl;

            // intakeHook.move_velocity(0); // Stops the intake so that all necessary calcuations can occur

            desiredIntakePosition = intake.get_position() + 16;


            intake.move_velocity(0); // Stops the intake so that all necessary calcuations can occur

            moveIntakeToDesiredPosition(TOP, desiredIntakePosition); // Moves a hook to its top position
        }

        // Same logic as the color sorter except for moving a hook to its neutral position
        if (global::findNextDown) { 
            // intakeHook.move_velocity(0);

            intake.move_velocity(0);

            moveIntakeToDesiredPosition(NEUTRAL);

            global::findNextDown = false;
        }

        // This is for storing a ring in the intake when moving towards a goal. Should help with making routes in autonomous and driver controller more convenient 
        if (global::storeRing) {
            // intakePre.move_velocity(600);
            // intakeHook.move_velocity(530); // 459

            intake.move_velocity(600);

            while (!checkForColor(false)) // Wait for a ring to come into the range of the color sensor
                pros::delay(5);

            // Stop the intake
            if (!global::overrideIntakeState)
                intake.move_velocity(600);
                // intakePre.move_velocity(0);
            // intakeHook.move_velocity(0);

            intake.move_velocity(0);

            global::storeRing = false;
        }


        */

        // std::cout << "Torque " << intake.get_torque() << std::endl;

        if (intake.get_torque() > 0.34 && !timerRunning) {
            timerRunning = true;
            jamStartTime = pros::millis();
        } else if (intake.get_torque() < 0.34 && timerRunning)
            timerRunning = false;

        if (timerRunning && pros::millis() - jamStartTime > 1000) {
            // std::cout << "Too hard stopping now" << std::endl;

            intake.move_velocity(-600);
            pros::delay(250);
            intake.move_velocity(intakeSpeed);
            timerRunning = false;
        }

        if (global::intakeState == states::intakeStates::Mogo) { // Sets the intake to 600 rpm, running directly to the mogo
            intakeSpeed = 600;
            // std::cout << intakeSpeed << std::endl;
            // std::cout << "Actual velocity is " << intake.get_actual_velocity_all()[0] << " target velocity is " << intake.get_target_velocity_all()[0] << std::endl;
        }
        else if (global::intakeState == states::intakeStates::Resting) { // Stops the intake
            intakeSpeed = 0;
            // std::cout << intakeSpeed << std::endl;
        }
        else if (global::intakeState == states::intakeStates::Reverse) // Reverses the intake at 600 rpm
            intakeSpeed = -600;
        

        intake.move_velocity(intakeSpeed);

        /*
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
            modifier += 10;
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
            modifier -= 10;

        controller.set_text(0, 5, std::to_string(modifier));
        */


        pros::delay(5); // A delay to ensure all processes run smoothly
    }
}




void handleArm() {
    int32_t settleTime;
    bool inMotion = false;
    bool removedBuffer = true;

    float targetArmPosition = 0;
    float currentArmPosition = 0;

    float armVelocity = 0;

    float error = 0; // slew is 30

    int counter = 0;

    float moreError = 0;

    std::initializer_list<float> exitRange = {5.0, 10.0};


    MyPID myPID(0.65, 0.0, 0.6, 0.0, &exitRange);
    
    while (true) {        
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

        currentArmPosition = (arm.get_position_all()[0] + arm.get_position_all()[1]) / 2 - 30;

        error = targetArmPosition - currentArmPosition;

        /*
        if (currentArmPosition != targetArmPosition)
            std::cout << "currentArm position is " << currentArmPosition << " target arm position " << targetArmPosition << std::endl;
        */
        // std::cout << error << std::endl;


        // if (error != 0)
            // std::cout << "error is " << error << " arm speed " << pid.update(error) << " my pid speed is " << myPID.update(error)<< std::endl;

        myPID.update(error);



        inMotion = !myPID.earlyExit();

        // std::cout << "Early exit? " << !myPID.earlyExit() << std::endl;

        if (inMotion) {
            armVelocity = myPID.getVelocity();

            arm.move_velocity(armVelocity);
        } else {
            // std::cout << "arm position before stop " << (arm.get_position_all()[0] + arm.get_position_all()[1]) / 2 << std::endl;
            arm.brake();

            // std::cout << "Waiting and not moving" << std::endl;

            if (!removedBuffer) {
                global::armStatesQueue.erase(global::armStatesQueue.begin());
                removedBuffer = true;
            }
            // std::cout << "arm position after stop " << (arm.get_position_all()[0] + arm.get_position_all()[1]) / 2 << std::endl;

            // counter = 0;
            // moreError = 0;

            if (global::armStatesQueue.size() != 0) {
                std::cout << "First in queue is " << global::armStatesQueue.begin()[0] << std::endl;
                global::armState = global::armStatesQueue.begin()[0];
                removedBuffer = false;
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