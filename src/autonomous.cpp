#include "main.h"

#include "lemlib/chassis/chassis.hpp"

#include "ports.h"
#include "pros/rtos.hpp"
#include "states.h"
#include <cmath>



void closeClamp(bool close)
{
    if (close)
        mogoClamp.extend();
    else
        mogoClamp.retract();
}

void autonomous() {
    optical.set_led_pwm(100);

    std::cout << "In autonomous" << std::endl;

    if (autonSelected == redPositiveCorner ) {

        base.setPose(-52.1, -60.5, 270);

        // Grab the goal

        base.moveToPose(-22.6, -60.5, 270, 1500, {.forwards = false, .minSpeed = 82.55, .earlyExitRange = 3});

        base.moveToPose(-8.9, -52.7, 237, 1000, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        // Go to positive corner

        base.moveToPose(-22.6, -60.5, 270, 1000, {.minSpeed = 82.55, .earlyExitRange = 3});

        base.moveToPose(-52.1, -60.5, 270, 1500);

        // Clear positive corner

        base.waitUntilDone();
        
        doinker.extend();

        base.turnToHeading(180, 1000);

        base.waitUntilDone();

        doinker.retract();


    }
    else if (autonSelected == redNegativeCorner) {
        // Negative corner auton
        
        // throw into initialize
        base.setPose(-52.1, 23.4, 270);

        pros::delay(500);

        base.moveToPoint(-31, 23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        base.swingToPoint(-23.5, 47.0, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 63.5, .earlyExitRange = 5});

        base.waitUntilDone();

        intakeState = intakeMogo;

        base.moveToPoint(-23.5, 47.0, 1000);

        base.swingToPoint(-3.6, 50.6, lemlib::DriveSide::LEFT, 1000);

        base.moveToPose(-15, 49.6, 70, 2000, {.maxSpeed = 63.5});

        pros::delay(500);

        /* broken code

        base.moveToPose(-24, 47, 75, 2000, {.forwards = false});

        base.turnToHeading(75, 1000);

        pros::delay(250);

        base.moveToPoint(-12.5, 44.4, 2000, {.maxSpeed = 63.5});

        pros::delay(250);

        */


        base.moveToPose(-23.5, 47, 180, 2500, {.forwards = false});

        // base.turnToPoint(23.5, -0.3, 1000);

        base.moveToPoint(-23.5, 9.9, 2000);

        base.waitUntil(16);
        
        intakeState = intakeResting;
    }
    else if (autonSelected == redSoloAWP) {
        // See SG-1
        base.setPose(-52.1, -23.4, 270); // Roughly 11 inches in height from wheel center

        base.moveToPoint(-31, -23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        intakeState = intakeMogo; // Start intaking rings to the mogo

        pros::delay(500);

        // Grab the second goal

        base.turnToPoint(-46.6, 9.8, 4000);

        base.waitUntilDone();

        intakeState = intakeResting;

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(-46.6, 9.8, 1000);

        base.turnToHeading(240, 2000);

        base.waitUntilDone();

        std::cout << base.getPose().theta << std::endl;

        base.moveToPose(-31, 19.5, 240, 2500, {.forwards = false});

        base.waitUntilDone();

        std::cout << "Point before goal is " << base.getPose().x << ", " << base.getPose().y << std::endl; 

        closeClamp(true);

        pros::delay(500);

        intakeState = intakeMogo;

        base.turnToPoint(-24, 48, 1500);

        base.moveToPoint(-24, 50.0, 1500);

        base.waitUntilDone();

        std::cout << "Point before goal is " << base.getPose().x << ", " << base.getPose().y << std::endl; 

        base.swingToPoint(-3.6, 50.6, lemlib::DriveSide::LEFT, 1000);

        base.moveToPoint(-14.8, 50.6, 1500);

        base.waitUntilDone();

        std::cout << "Point before goal is " << base.getPose().x << ", " << base.getPose().y << std::endl; 

        pros::delay(250);

        base.moveToPose(-24, 48, 180, 1500, {.forwards = false});

        /*
        base.turnToPoint(-14.8, 52, 1500);

        base.moveToPoint(-14.8, 52, 2000);

        base.waitUntilDone();

        */


        base.moveToPose(-24, 9.3, 180, 2000, {.minSpeed = 63.5});


        /*

        base.moveToPose(-15, 49.6, 70, 2000, {.maxSpeed = 63.5});

        pros::delay(500);

        base.moveToPose(-23.4, 47.3, 180, 1500);

        base.turnToPoint(15, 46.3, 1000);
    
        base.moveToPoint(-15, 46.3, 1000);
        */
    }
    else if (autonSelected == blueNegativeCorner) {
        // throw into initialize
        base.setPose(62.7, 23.4, 90);

        pros::delay(500);

        base.moveToPoint(31, 23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        mogoClamp.extend();

        pros::delay(500);

        base.swingToPoint(23.5, 47.0, lemlib::DriveSide::RIGHT, 1000);

        base.waitUntilDone();

        intakeState = intakeMogo;

        base.moveToPoint(23.5, 47.0, 1000);

        base.swingToPoint(3.6, 50.6, lemlib::DriveSide::RIGHT, 1000);

        base.moveToPose(9.1, 49.6, 250, 1000);

        pros::delay(500);

        base.moveToPose(24, 47, 260, 2000, {.forwards = false});

        base.turnToHeading(260, 1000);

        pros::delay(250);

        base.moveToPoint(9.1, 44.4, 1000);

        pros::delay(250);


        base.moveToPose(23.5, 47, 180, 2500, {.forwards = false});

        base.turnToPoint(23.5, -0.3, 1000);

        base.moveToPoint(23.5, 9.9, 2000);

        base.waitUntil(16);
        
        intakeState = intakeResting;

        mogoClamp.retract();
    }
    else if (autonSelected == bluePositiveCorner) {
    }
    else if (autonSelected == skills) {

        base.setPose(-57.8, -17, 302);

        // Get mogo

        base.moveToPoint(-55.6, -18.4, 1000, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        // Grab 2 rings

        base.swingToHeading(90, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .minSpeed = 76.2, .earlyExitRange = 10});
    
        base.waitUntilDone();

        intakeState = intakeMogo;

        base.moveToPose(-24, -24, 135, 1500, {.minSpeed = 76.2, .earlyExitRange = 3});

        base.swingToPoint(-24, -48, lemlib::DriveSide::RIGHT, 1000, {.minSpeed =76.2, .earlyExitRange = 10});

        base.moveToPose(-24, -41.5, 180, 1000, {.maxSpeed = 44.45, .minSpeed = 40.45, .earlyExitRange = 3});

        // Grab 3 rings

        base.swingToHeading(270, lemlib::DriveSide::RIGHT, 1000);

        base.moveToPose(-57.8, -48, 270, 3500);

        pros::delay(500);

        base.moveToPoint(-51.8, -48, 1000, {.forwards = false});

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 76.2});

        // Drop goal in a corner

        base.moveToPose(-59.1, -60.9, 90, 1500, {.forwards = false});
        
        base.waitUntilDone();

        pros::delay(500);

        intakeState = intakeResting;

        findNextDown = true;

        pros::delay(500);

        closeClamp(false);

        pros::delay(500);

        // Go to center and grab a ring

        base.moveToPose(-36, -48, 0, 1500, {.minSpeed = 63.5, .earlyExitRange = 3});

        base.swingToPoint(0, 0, lemlib::DriveSide::RIGHT, 1000, {.minSpeed = 82.55, .earlyExitRange = 10});

        storeRing = true;

        base.moveToPose(0, 0, 45, 3000);

        base.waitUntilDone();

        // Grab another ring

        overrideIntakeState = true;

        intakePre.move_velocity(0);

        base.swingToPoint(-16.9, 16.9, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 44.45, .earlyExitRange = 10});

        base.moveToPose(-16.9, 16.9, 315, 1500);

        base.waitUntilDone();

        overrideIntakeState = false;

        // Grab another goal

        base.swingToHeading(115, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .minSpeed = 44.45, .earlyExitRange = 10});

        base.moveToPose(-37.3, 18.2, 115, 3500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        // Grab a ring

        intakeState = intakeMogo;

        base.swingToPoint(-24, 48, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .minSpeed = 76.2, .earlyExitRange = 10});
   
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

        intakeState = intakeResting;

        findNextDown = true;

        closeClamp(false);

        pros::delay(500);










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

        intakeState = intakeMogo;

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

        intakeState = intakeResting;

        findNextDown = true;

        pros::delay(500);

        closeClamp(false);
    }
    else if (autonSelected == none) {
    }

    optical.set_led_pwm(0);
}