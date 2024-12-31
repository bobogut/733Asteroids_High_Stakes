#include "main.h"

#include "lemlib/chassis/chassis.hpp"

#include "ports.h"
#include "pros/rtos.hpp"
#include "states.h"
#include <cmath>
#include <cstdint>



void closeClamp(bool close) {
    // I just created this so that I do not get confused
    if (close)
        mogoClamp.extend();
    else
        mogoClamp.retract();
}

void autonomous() {
    if (!initializedBrainCoords) {
        pros::lcd::initialize(); // initialize brain screen

        pros::Task screen_task([&]() {
            while (true) {
                // print robot location to the brain screen
                pros::lcd::print(0, "X: %f", base.getPose().x);
                pros::lcd::print(1, "Y: %f", base.getPose().y);
                pros::lcd::print(2, "Theta: %f", base.getPose().theta);
                // delay to save resources
                pros::delay(20);
            }
        });

        initializedBrainCoords = true;
    }

    optical.set_led_pwm(100);

    // std::ofstream debugger;
    // debugger.open("debugger.txt");

    std::cout << "In autonomous" << std::endl;

    int32_t startTime = pros::millis();



    leftMotors.set_brake_mode_all(pros::v5::MotorBrake::hold);
	rightMotors.set_brake_mode_all(pros::v5::MotorBrake::hold);



    if (autonSelected == REDPOSITIVECORNER ) {
        base.setPose(-51.3, -24, 270);

        base.moveToPose(-27, -24, 270, 3000, {.forwards = false, .minSpeed = 63.5});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(-24, -48, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-24, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(-53.1, 11.5, 1500);

        base.moveToPoint(-51.1, 11.5, 3000);
        /*
        base.turnToPoint(-8.8, -28.2, 1000);

        base.waitUntilDone();

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(-8.8, -28.2, 3000);

        base.waitUntilDone();

        std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        */
    } else if (autonSelected == REDNEGATIVECORNER) {
        base.setPose(-51.6, 36.2, 90);

        storeRing = true;

        overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(-11.0, 36.2, 270, 3000, {.minSpeed = 44.45, .earlyExitRange = 2});

        base.swingToHeading(0, lemlib::DriveSide::LEFT, 1000);

        base.waitUntilDone();

        std::cout << "Turned at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(base.getPose().x, 54.4, 0, 2000);

        base.waitUntil(14);

        overrideIntakeState = false;

        base.moveToPose(base.getPose().x, 37.0, 0, 2000, {.forwards = false});

        std::cout << "Done storing two rings at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.swingToHeading(5, lemlib::DriveSide::RIGHT, 1500, {.minSpeed = 31.75, .earlyExitRange = 2});

        base.waitUntilDone();

        std::cout << "First swing at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(-24, 24, 2000, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Got the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(-24, 48, 1000);

        base.waitUntilDone();

        pros::delay(500);

        intakeState = INTAKEMOGO;

        base.moveToPoint(-24, 52, 1000);

        base.turnToPoint(-60.1, 36.2, 1000);

        base.moveToPoint(-60.1, 36.2, 3000);

        // base.turnToPoint(42.4, -13.3, 1000);

        // base.moveToPoint(42.4, -13.3, 2500);

        base.waitUntilDone();


    } else if (autonSelected == REDSOLOAWP) {
        // See SG-1
        base.setPose(-51.3, -24, 270);

        base.moveToPose(-27, -24, 270, 3000, {.forwards = false, .minSpeed = 63.5});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(-24, -48, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-24, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(-45.4, 23.0, 1000, {.forwards = false});

        base.moveToPose(-45.4, 23.0, 163, 2500, {.forwards = false});

        base.waitUntilDone();

        intakeState = INTAKERESTING;

        findNextDown = true;

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(-41.9, 11.7, 1000);

        std::cout << "Done repositionining at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(-24, 24, 1000, {.forwards = false}); // A heading of 236

        base.waitUntilDone();

        base.moveToPose(-24, 24, 236, 1500, {.forwards = false});

        base.waitUntil(12);

        closeClamp(true);

        base.waitUntilDone();

        std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        intakeState = INTAKEMOGO;

        base.turnToPoint(-26.0, 48.0, 1000);

        base.moveToPose(-24.0, 48.0, 0, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(-24, 9.3, 180, 2000, {.minSpeed = 63.5});

        base.waitUntilDone();

        doinker.extend();

        std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (autonSelected == BLUEPOSITIVECORNER) {
        base.setPose(51.3, -24, 90);

        base.moveToPose(27, -24, 90, 3000, {.forwards = false, .minSpeed = 63.5});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(24, -48, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(24, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(8.8, -28.2, 1000);

        base.waitUntilDone();

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(8.8, -28.2, 3000);

        base.waitUntilDone();

        std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (autonSelected == BLUENEGATIVECORNER) {
        /*
        base.setPose(51.3, 24, 90);

        base.moveToPose(27, 24, 90, 3000, {.forwards = false, .minSpeed = 63.5});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(24, 48, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(24, 48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        */
        // Negative corner auton
        base.setPose(51.6, 36.2, 270);

        storeRing = true;

        overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(11.0, 36.2, 270, 3000, {.minSpeed = 25.4, .earlyExitRange = 2});

        base.swingToHeading(0, lemlib::DriveSide::RIGHT, 1000);

        base.waitUntilDone();

        std::cout << "Turned at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(base.getPose().x, 54.4, 0, 2000);

        base.waitUntil(14);

        overrideIntakeState = false;

        base.moveToPose(base.getPose().x, 37.0, 0, 2000, {.forwards = false});

        std::cout << "Done storing two rings at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.swingToHeading(355, lemlib::DriveSide::LEFT, 1500, {.minSpeed = 31.75, .earlyExitRange = 2});

        base.waitUntilDone();

        std::cout << "First swing at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(24, 24, 2000, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Got the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(24, 48, 1000);

        base.waitUntilDone();

        pros::delay(500);

        intakeState = INTAKEMOGO;

        base.moveToPoint(24, 52, 1000);

        base.turnToPoint(60.1, 36.2, 1000);

        base.moveToPoint(60.1, 36.2, 3000);

        // base.turnToPoint(42.4, -13.3, 1000);

        // base.moveToPoint(42.4, -13.3, 2500);

        base.waitUntilDone();

        std::cout << "Finished at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        /*
        // throw into initialize
        base.setPose(52.1, 23.4, 90);

        base.moveToPoint(31, 23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        base.swingToPoint(23.5, 47.0, lemlib::DriveSide::RIGHT, 1000, {.minSpeed = 63.5, .earlyExitRange = 5});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(23.5, 47.0, 1000);

        base.swingToPoint(3.6, 50.6, lemlib::DriveSide::RIGHT, 1000);

        base.moveToPose(15, 49.6, 290, 2000, {.maxSpeed = 63.5});

        pros::delay(500);

        base.moveToPose(23.5, 47, 180, 2500, {.forwards = false});

        // base.turnToPoint(23.5, -0.3, 1000);

        base.moveToPoint(23.5, 9.9, 2000);

        base.waitUntil(16);
        
        intakeState = INTAKERESTING;
        */
    } else if (autonSelected == BLUESOLOAWP) {
        // See SG-1
        base.setPose(51.3, -24, 90);

        base.moveToPose(27, -24, 90, 3000, {.forwards = false, .minSpeed = 63.5});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(24, -48, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(24, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(45.4, 23.0, 1000, {.forwards = false});

        base.moveToPose(45.4, 23.0, 197, 2500, {.forwards = false});

        base.waitUntilDone();

        intakeState = INTAKERESTING;

        findNextDown = true;

        closeClamp(false);

        base.moveToPoint(41.9, 11.7, 1000);

        std::cout << "Done repositionining at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(24, 24, 1000, {.forwards = false}); // A heading of 236

        base.waitUntilDone();

        base.moveToPose(24, 24, 124, 1000, {.forwards = false});

        base.waitUntil(12);

        closeClamp(true);

        base.waitUntilDone();

        std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        intakeState = INTAKEMOGO;

        base.turnToPoint(26.0, 48.0, 1000);

        base.moveToPose(24.0, 48.0, 0, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(24, 9.3, 180, 2000, {.minSpeed = 63.5});

        base.waitUntilDone();

        doinker.extend();

        std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (autonSelected == SKILLS) {
        base.setPose(-57.8, -17, 302);

        // Get mogo

        base.moveToPoint(-55.6, -18.4, 1000, {.forwards = false});

        base.waitUntilDone();

        std::cout << "Done grabbing the first goal at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        closeClamp(true);

        pros::delay(250);

        // Grab 2 rings

        base.turnToPoint(-24, -24, 750);
    
        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-31.0, -22.6, 1500, {.minSpeed = 76.2, .earlyExitRange = 3});

        base.turnToPoint(-24, -48, 2000);

        base.waitUntilDone();

        std::cout << "Done turning at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(-24, -48, 1500, {.minSpeed = 35, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "Done grabbing rings two and three at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // findNextDown = true;

        // Grab 3 rings

        base.swingToPoint(-48, -48, lemlib::DriveSide::RIGHT, 1000, {.minSpeed = 63.5, .earlyExitRange = 5});

        base.moveToPose(-59.8, -48, 270, 2500);

        base.waitUntilDone();

        std::cout << "Done grabbing rings four and five at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(-51.8, -48, 1500, {.forwards = false});

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 76.2});

        base.waitUntilDone();

        std::cout << "Done grabbing ring six at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(-52.5, -58.9, 90, 1000);

        // Drop goal in a corner, goal adds another 7 in in length

        base.moveToPose(-54.5, -60.9, 90, 1000, {.forwards = false});
        
        base.waitUntilDone();

        intakeState = INTAKERESTING;

        findNextDown = true;

        closeClamp(false);

        pros::delay(250);

        std::cout << "Dropped the goal of in the corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Go to center and grab a ring

        // base.moveToPose(-36, -48, 0, 1500, {.minSpeed = 63.5, .earlyExitRange = 3});

        base.swingToPoint(0, 0, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .minSpeed = 83, .earlyExitRange = 5});

        storeRing = true;

        base.moveToPose(0, 0, 45, 3000);

        base.waitUntilDone();

        std::cout << "At center at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        base.turnToPoint(-16.9, 16.9, 1000, {.minSpeed = 44, .earlyExitRange = 3});

        base.waitUntilDone();

        overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(-24, 24, 315, 1500);

        base.waitUntilDone();

        overrideIntakeState = false;

        std::cout << "Grabbed rings seven and eight at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another goal

        base.turnToPoint(-37.3, 23.6, 1000, {.forwards = false});

        base.moveToPose(-37.3, 23.6, 90, 3500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        std::cout << "Grabbed second goal at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab a ring

        intakeState = INTAKEMOGO;

        base.swingToPoint(-24, 48, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .minSpeed = 76.2, .earlyExitRange = 5});
   
        base.moveToPose(-28.5, 37.9, 23, 1500, {.minSpeed = 76.2, .earlyExitRange = 3});

        // Grab 3 rings

        base.swingToHeading(270, lemlib::DriveSide::LEFT, 1000);

        base.moveToPose(-57.8, 48, 270, 3500);

        pros::delay(500);

        base.moveToPoint(-51.8, 48, 1000, {.forwards = false});

        base.swingToHeading(90, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 76.2});

        // Drop a second goal in a corner

        base.moveToPose(-59.1, 60.9, 90, 1500, {.forwards = false});
        
        base.waitUntilDone();

        intakeState = INTAKERESTING;

        findNextDown = true;

        closeClamp(false);

        pros::delay(250);

        std::cout << "Dropped second goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;









        /*

        // Grab another ring

        base.moveToPose(0, 40, 125, 2500, {.minSpeed = 88.9, .earlyExitRange = 3});

        base.waitUntilDone();
        
        overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(24, 24, 125, 1000, {.minSpeed = 88.9, .earlyExitRange = 3});

        // Shove a goal into a corner

        base.waitUntilDone();

        overrideIntakeState = false;

        base.moveToPose(48, 18, 104, 1000, {.minSpeed = 88.9, .earlyExitRange = 10});

        base.swingToPoint(59, 53.1, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 88.9, .earlyExitRange = 3});

        base.moveToPose(59, 53.1, 7, 1500);

        // Grab the third goal

        base.moveToPose(59.3, 21.4, 0, 1500, {.forwards = false, .minSpeed = 88.9, .earlyExitRange = 3});

        base.moveToPose(52.5, 9.3, 30, 1000);

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        // Grab 5 rings

        base.turnToPoint(24, -24, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(24, -24, 1500, {.minSpeed = 76.2, .earlyExitRange = 3});

        base.swingToPoint(24, -48, lemlib::DriveSide::LEFT, 1000, {.minSpeed =76.2, .earlyExitRange = 10});

        base.moveToPose(24, -41.5, 180, 1000, {.maxSpeed = 44.45, .minSpeed = 40.45, .earlyExitRange = 3});

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000);

        base.moveToPose(57.8, -48, 270, 3500);

        pros::delay(500);

        base.moveToPoint(51.8, -48, 1000, {.forwards = false});

        base.swingToHeading(270, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed = 76.2});

        // Drop a fourth goal in a corner

        base.moveToPose(59.1, -60.9, 270, 1500, {.forwards = false});
        
        base.waitUntilDone();

        pros::delay(500);

        intakeState = INTAKERESTING;

        findNextDown = true;

        pros::delay(500);

        closeClamp(false);

        */

        base.moveToPose(0, 40, 125, 4000, {.minSpeed = 88.9, .earlyExitRange = 3});

        base.moveToPose(24, 24, 125, 1000);

        base.moveToPose(51.9, 12.3, 112, 1500);

        base.swingToPoint(60.9, 52.7, lemlib::DriveSide::LEFT, 1000);

        base.waitUntilDone();

        std::cout << "Got to third goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(60.9, 52.7, 3000);

        base.waitUntilDone();

        std::cout << "Shoved third goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(59.0, -13.8, 1, 4000);

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Got fourth goal at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(61.2, -53.9, 356, 3000);

        closeClamp(false);

        findNextDown = true;
        
        pros::delay(250);

        std::cout << "Dropped fourth goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(59.0, -13.8, 1, 4000);

        base.waitUntilDone();

        std::cout << "Done with skills at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    } else if (autonSelected == NONE) {
        /*
        base.setPose(0, 0, 0);

        base.turnToHeading(90, 5000);

        base.waitUntilDone();

        std::cout << "Base at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        */
        
        /*
        base.setPose(0, 0, 0);

        std::cout << "Base at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(0, 72, 5000);

        base.waitUntilDone();

        std::cout << "Base at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;


        base.turnToHeading(180, 1000);
        */

        /*
        base.setPose(15.4, 29.6, 0);

        base.swingToHeading(350, lemlib::DriveSide::LEFT, 1500, {.minSpeed = 31.75, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "First swing at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(7.9, 35.8, 350, 1000, {.minSpeed = 19.05, .earlyExitRange = 1});

        base.waitUntilDone();

        std::cout << "Inched back at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.swingToPoint(24, 24, lemlib::DriveSide::RIGHT, 1500, {.forwards = false});

        base.waitUntilDone();

        std::cout << "Second swing to goal at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(24, 24, 307, 1500, {.forwards = false});

        base.waitUntil(10);

        std::cout << "Closing clamp at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        closeClamp(true);

        pros::delay(250);

        intakeState = INTAKEMOGO;

        base.turnToPoint(24, 48, 1000);

        base.moveToPose(24, 48, 0, 1000, {.minSpeed = 101.6, .earlyExitRange = 5});

        base.waitUntilDone();

        std::cout << "Got third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Get preload

        base.swingToPoint(55.1, 36.2, lemlib::DriveSide::RIGHT, 1000);

        base.moveToPoint(55.1, 36.2, 2000); 

        base.waitUntilDone();

        std::cout << "Got preload ring (4th) at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(44.6, -11.5, 1000);

        base.moveToPose(44.6, -11.5, 193, 3000);
        */

        /*
        base.turnToPoint(39.2, 40, 1000);

        base.moveToPose(39.2, 40, 0, 5000);

        base.waitUntilDone();

        std::cout << "At second corner at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(-39.2, 40, 1000);

        base.moveToPose(-39.2, 40, 270, 5000);

        base.waitUntilDone();

        std::cout << "At third corner at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(-39.2, -40, 1000);

        base.moveToPose(-39.2, -40, 180, 5000);

        base.waitUntilDone();

        std::cout << "At fourth corner at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(39.2, -40, 1000);

        base.waitUntilDone();

        std::cout << "Done at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        */
    }

    optical.set_led_pwm(0);
}