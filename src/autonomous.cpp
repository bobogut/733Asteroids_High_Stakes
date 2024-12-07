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
    optical.set_led_pwm(100);

    std::cout << "In autonomous" << std::endl;

    int32_t startTime = pros::millis();

    if (autonSelected == REDPOSITIVECORNER ) {
        base.setPose(-51.7, -30.7, 270);

        base.moveToPoint(-37.7, -30.7, 1000, {.forwards = false, .maxSpeed = 69.85, .minSpeed = 44.45, .earlyExitRange = 5});

        base.moveToPose(-29.4, -26.6, 235, 2500, {.forwards = false, .maxSpeed = 69.85});

        base.waitUntil(10.5);

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(-30, -48, 1000, {.minSpeed = 95});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-30, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        intakeState = INTAKERESTING;

        /*

        base.turnToPoint(-60, -53.8, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-60, -53.8, 3000);

        base.turnToHeading(230, 1000);

        base.waitUntilDone();

        std::cout << "Got to the corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        doinker.extend();

        pros::delay(500);

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000);

        */

        base.turnToPoint(-9.6, 26.2, 1000);

        base.waitUntilDone();

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(-9.6, -26.2, 3000);

        std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (autonSelected == REDNEGATIVECORNER) {
        // Negative corner auton
        
        // throw into initialize
        base.setPose(-52.1, 23.4, 270);

        base.moveToPoint(-31, 23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        base.swingToPoint(-23.5, 47.0, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 63.5, .earlyExitRange = 5});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

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
        
        intakeState = INTAKERESTING;
    } else if (autonSelected == REDSOLOAWP) {
        // See SG-1
        base.setPose(-51.7, -30.7, 270);

        base.setPose(-51.7, -30.7, 270);

        base.moveToPoint(-37.7, -30.7, 1000, {.forwards = false, .maxSpeed = 69.85, .minSpeed = 44.45, .earlyExitRange = 5});

        base.moveToPose(-29.4, -26.6, 235, 2500, {.forwards = false, .maxSpeed = 69.85});

        base.waitUntil(10.5);

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(-30, -48, 1000, {.minSpeed = 95});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-30, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        pros::delay(500);

        base.moveToPose(-50.4, 5.8, 335, 3000);

        base.waitUntil(48);

        closeClamp(false);

        intakeState = INTAKERESTING;

        base.waitUntilDone();

        std::cout << "Done getting the third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        /*

        base.setPose(-52.1, -23.4, 270); // Roughly 11 inches in height from wheel center

        base.moveToPoint(-31, -23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        base.turnToPoint(-44.9, 9.8, 4000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO; // Start intaking rings to the mogo

        pros::delay(500);


        // Grab the second goal

        intakeState = INTAKERESTING;

        closeClamp(false);

        findNextDown = true;

        pros::delay(500);

        base.moveToPoint(-44.9, 9.8, 1000);

        */


        base.turnToHeading(235, 2000);

        base.waitUntilDone();

        std::cout << base.getPose().theta << std::endl;

        base.moveToPose(-31, 22, 235, 2500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        intakeState = INTAKEMOGO;

        // base.turnToPoint(-24, 48, 1500);

        base.turnToPoint(-36, 50.0, 1000);

        base.moveToPoint(-36, 50.0, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the fourth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.swingToPoint(-3.6, 50.6, lemlib::DriveSide::LEFT, 1000);

        base.moveToPoint(-28, 50.6, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the fifth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        pros::delay(250);

        base.moveToPose(-30, 48, 180, 1500, {.forwards = false});

        /*
        base.turnToPoint(-14.8, 52, 1500);

        base.moveToPoint(-14.8, 52, 2000);

        base.waitUntilDone();

        */


        base.moveToPose(-30, 9.3, 180, 2000, {.minSpeed = 63.5});

        std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;



        /*

        base.moveToPose(-15, 49.6, 70, 2000, {.maxSpeed = 63.5});

        pros::delay(500);

        base.moveToPose(-23.4, 47.3, 180, 1500);

        base.turnToPoint(15, 46.3, 1000);
    
        base.moveToPoint(-15, 46.3, 1000);
        */
    } else if (autonSelected == BLUEPOSITIVECORNER) {
        base.setPose(51.7, -30.7, 90);

        base.moveToPoint(37.7, -30.7, 1000, {.forwards = false, .maxSpeed = 69.85, .minSpeed = 44.45, .earlyExitRange = 5});

        base.moveToPose(29.4, -26.6, 125, 2500, {.forwards = false, .maxSpeed = 69.85});

        base.waitUntil(10.5);

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(30, -48, 1000, {.minSpeed = 95});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(30, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        intakeState = INTAKERESTING;

        /*

        base.turnToPoint(-60, -53.8, 1000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(-60, -53.8, 3000);

        base.turnToHeading(230, 1000);

        base.waitUntilDone();

        std::cout << "Got to the corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        doinker.extend();

        pros::delay(500);

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000);

        */

        base.turnToPoint(9.6, 26.2, 1000);

        base.waitUntilDone();

        closeClamp(false);

        pros::delay(250);

        base.moveToPoint(9.6, -26.2, 3000);

        std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (autonSelected == BLUENEGATIVECORNER) {
        // Negative corner auton
        
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

        /* broken code

        base.moveToPose(-24, 47, 75, 2000, {.forwards = false});

        base.turnToHeading(75, 1000);

        pros::delay(250);

        base.moveToPoint(-12.5, 44.4, 2000, {.maxSpeed = 63.5});

        pros::delay(250);

        */


        base.moveToPose(23.5, 47, 180, 2500, {.forwards = false});

        // base.turnToPoint(23.5, -0.3, 1000);

        base.moveToPoint(23.5, 9.9, 2000);

        base.waitUntil(16);
        
        intakeState = INTAKERESTING;
    } else if (autonSelected == BLUESOLOAWP) {
        base.setPose(51.7, -30.7, 90);

        base.setPose(51.7, -30.7, 90);

        base.moveToPoint(37.7, -30.7, 1000, {.forwards = false, .maxSpeed = 69.85, .minSpeed = 44.45, .earlyExitRange = 5});

        base.moveToPose(29.4, -26.6, 145, 2500, {.forwards = false, .maxSpeed = 69.85});

        base.waitUntil(10.5);

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(30, -48, 1000, {.minSpeed = 95});

        base.waitUntilDone();

        intakeState = INTAKEMOGO;

        base.moveToPoint(30, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        pros::delay(500);

        base.moveToPose(50.4, 5.8, 25, 3000);

        base.waitUntil(48);

        closeClamp(false);

        intakeState = INTAKERESTING;

        base.waitUntilDone();

        std::cout << "Done getting the third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        /*

        base.setPose(-52.1, -23.4, 270); // Roughly 11 inches in height from wheel center

        base.moveToPoint(-31, -23.4, 1000, {.forwards = false, .maxSpeed = 82.55});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        base.turnToPoint(-44.9, 9.8, 4000);

        base.waitUntilDone();

        intakeState = INTAKEMOGO; // Start intaking rings to the mogo

        pros::delay(500);


        // Grab the second goal

        intakeState = INTAKERESTING;

        closeClamp(false);

        findNextDown = true;

        pros::delay(500);

        base.moveToPoint(-44.9, 9.8, 1000);

        */


        base.turnToHeading(235, 2000);

        base.waitUntilDone();

        std::cout << base.getPose().theta << std::endl;

        base.moveToPose(31, 22, 125, 2500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        intakeState = INTAKEMOGO;

        // base.turnToPoint(-24, 48, 1500);

        base.turnToPoint(36, 50.0, 1000);

        base.moveToPoint(36, 50.0, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the fourth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.swingToPoint(3.6, 50.6, lemlib::DriveSide::LEFT, 1000);

        base.moveToPoint(28, 50.6, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the fifth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        pros::delay(250);

        base.moveToPose(30, 48, 180, 1500, {.forwards = false});

        /*
        base.turnToPoint(-14.8, 52, 1500);

        base.moveToPoint(-14.8, 52, 2000);

        base.waitUntilDone();

        */


        base.moveToPose(30, 9.3, 180, 2000, {.minSpeed = 63.5});

        std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    }
    else if (autonSelected == SKILLS) {

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

        base.turnToHeading(180, 2000);

        base.waitUntilDone();

        std::cout << "Done turning at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;


        base.moveToPose(-31, -41.5, 180, 1500, {.minSpeed = 35, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "Done grabbing rings two and three at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // findNextDown = true;

        // Grab 3 rings

        base.swingToPoint(-48, -48, lemlib::DriveSide::RIGHT, 1000, {.minSpeed = 63.5, .earlyExitRange = 5});

        base.moveToPose(-59.8, -48, 270, 4000);

        base.waitUntilDone();

        std::cout << "Done grabbing rings four and five at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        pros::delay(500);

        base.moveToPoint(-51.8, -48, 1500, {.forwards = false});

        base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 76.2});

        base.waitUntilDone();

        std::cout << "Done grabbing ring six at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

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

        base.swingToPoint(-16.9, 16.9, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 44.45, .earlyExitRange = 3});

        base.waitUntilDone();

        overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(-24, 24, 315, 1500);

        base.waitUntilDone();

        overrideIntakeState = false;

        std::cout << "Grabbed rings seven and eight at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another goal

        base.turnToHeading(90, 1000);

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

    }
    else if (autonSelected == NONE) {
        storeRing = true;
    }

    optical.set_led_pwm(0);
}