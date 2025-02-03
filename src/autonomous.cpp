#include "global_var.h"
#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "ports.h"
#include "states.h"
#include "brain_screen.h"



void closeClamp(bool close) {
    // I just created this so that I do not get confused
    if (close)
        mogoClamp.extend();
    else
        mogoClamp.retract();
}



float reflectAngle(float originalAngle) {
    return 360 - originalAngle;
}

float reflectX(float originalX) {
    return -originalX;
}

lemlib::DriveSide reflectDriveSide(lemlib::DriveSide lockedSide) {
    if (lockedSide == lemlib::DriveSide::LEFT)
        return lemlib::DriveSide::RIGHT;
    else
        return  lemlib::DriveSide::LEFT;
}



namespace coordinateInfo {
    class Point {
        public:
            float x;
            float y;

            void reflect() {
                x = reflectX(x);
            }
    };

    class Pose {
        public:
            float x;
            float y;
            float theta;
        
            void reflect() {
                x = reflectX(x);
                theta = reflectAngle(theta);
            }
    };

    class SwingPoint {
        public:
            float x;
            float y;
            lemlib::DriveSide lockedSide;

            void reflect() {
                x = reflectX(x);
                lockedSide = reflectDriveSide(lockedSide);
            }
    };

    class SwingTheta {
        public:
            float theta;
            lemlib::DriveSide lockedSide;

            void reflect() {
                theta = reflectAngle(theta);
                lockedSide = reflectDriveSide(lockedSide);
            }
    };
}



void positiveCornerRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    coordinateInfo::Pose startingPosition(-51.3, -24, 270);
    coordinateInfo::Pose safeMogo(-30, -24, 270);
    coordinateInfo::Point secondRing(-24, -48);
    coordinateInfo::Point ladder(-24, -9.3);

    if (blueSide) {
        startingPosition.reflect();
        safeMogo.reflect();
        secondRing.reflect();
        ladder.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPose(safeMogo.x, safeMogo.y, safeMogo.theta, 1500, {.forwards = false});

    base.waitUntilDone();

    closeClamp(true);

    pros::delay(250);

    std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    
    base.turnToPoint(secondRing.x, secondRing.y, 1000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(secondRing.x, secondRing.y, 1500);

    base.waitUntilDone();

    std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    /*
    base.turnToPoint(-60, -53.8, 1000);

    base.waitUntilDone();

   global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(-60, -53.8, 3000);

    base.turnToHeading(230, 1000);

    base.waitUntilDone();

    std::cout << "Got to the corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    doinker.extend();

    pros::delay(500);

    base.swingToHeading(90, lemlib::DriveSide::LEFT, 1000);

    */

    // base.turnToPoint(ladder.x, ladder.y, 1000);

    // base.waitUntilDone();

    // closeClamp(false);

    // pros::delay(250);

    // base.moveToPoint(ladder.x, ladder.y, 3000);


    base.turnToPoint(ladder.x, ladder.y, 1000);

    base.moveToPose(ladder.x, ladder.y, 0, 1500);

    // std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
}

void goalRushRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    coordinateInfo::Pose startingPosition(-51.6, -36.2, 90);
    coordinateInfo::Point prepTurn(-18.8, -36.2);
    coordinateInfo::Point grabGoal(-14.9, -41.1);

    if (blueSide) {
        startingPosition.reflect();
        prepTurn.reflect();
        grabGoal.reflect();
        // thirdRing.reflect();
        // preload.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    global::storeRing = true;

    base.moveToPoint(19, -36.2, 3000, {.minSpeed = 44.45, .earlyExitRange = 2});

    // base.swingToHeading(219, lemlib::DriveSide::LEFT, 1000);

    base.swingToPoint(-5, -69, lemlib::DriveSide::LEFT, 2000);

    base.waitUntilDone();

    doinker.toggle();

    pros::delay(100);

    // base.moveToPose(14.9, -41.1, 205, 1500);

    // base.waitUntil(2);

    // doinker.toggle();
}



void safeNegativeCornerRoutine(bool blueSide) {
    // Negative corner auton

    int32_t startTime = pros::millis();

    coordinateInfo::Pose startingPosition(-51.3, 24, 270);
    coordinateInfo::Pose negMogo(-30, 24, 270);
    coordinateInfo::Point secondRing(-24, 48);
    coordinateInfo::Pose ladder(-24, 9.3, 180);

    if (blueSide) {
        startingPosition.reflect();
        negMogo.reflect();
        secondRing.reflect();
        ladder.reflect();
    }
        
    // throw into initialize
    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPoint(negMogo.x, negMogo.y, 1500, {.forwards = false, .maxSpeed = 82.55});

    base.waitUntil(19);

    closeClamp(true);

    pros::delay(250);

    base.turnToPoint(secondRing.x, secondRing.y, 1000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(secondRing.x, secondRing.y, 1500);

    // base.swingToPoint(-3.6, 50.6, lemlib::DriveSide::LEFT, 1000);

    // base.moveToPose(-15, 49.6, 70, 2000, {.maxSpeed = 63.5});

    // pros::delay(500);

    // base.moveToPose(-23.5, 47, 180, 2500, {.forwards = false});

    // base.turnToPoint(23.5, -0.3, 1000);

    // base.moveToPoint(-23.5, 9.9, 2000);

    // base.waitUntil(16);
    
    // global::intakeState = states::intakeStates::Resting;

    base.turnToPoint(ladder.x, ladder.y, 1000);

    base.moveToPose(ladder.x, ladder.y, 180, 1500);
}

void ringRushRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    coordinateInfo::Pose startingPosition(-51.6, 36.2, 90);
    coordinateInfo::Point prepTurn(-11.0, 36.2);
    coordinateInfo::Point grabGoal(-14.9, 41.1);
    coordinateInfo::SwingTheta wall(0, lemlib::DriveSide::LEFT);
    float secondRingY = 45;
    coordinateInfo::SwingTheta mogoHeading(10, lemlib::DriveSide::RIGHT);
    coordinateInfo::Pose mogoPose(-18, 30, 10);
    coordinateInfo::Point thirdRing(-24, 40);
    coordinateInfo::SwingPoint corner(-61, 62, lemlib::DriveSide::LEFT);
    coordinateInfo::Point reposition(-54, 57.8);
    coordinateInfo::Point sixthRing(-48.9, 8.7);
    coordinateInfo::SwingPoint allianceStakeHeading(-72.0, 0.0, lemlib::DriveSide::RIGHT);
    coordinateInfo::Point allianceStake(-58.0, 0.0);

    if (blueSide) {
        startingPosition.reflect();
        prepTurn.reflect();
        grabGoal.reflect();
        wall.reflect();
        secondRingY = -secondRingY;
        mogoHeading.reflect();
        // corner.reflect(corner);

        corner.reflect();

        reposition.reflect();
        sixthRing.reflect();
        allianceStakeHeading.reflect();
        allianceStake.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    global::storeRing = true;

    base.moveToPoint(prepTurn.x, prepTurn.y, 3000, {.minSpeed = 44.45, .earlyExitRange = 2});

    base.swingToHeading(wall.theta, wall.lockedSide, 1000);

    base.waitUntilDone();

    base.moveToPose(base.getPose().x, secondRingY, 0, 2000);

    base.waitUntilDone();

    std::cout << "Done storing two rings at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.swingToHeading(mogoHeading.theta, mogoHeading.lockedSide, 1500, {.minSpeed = 31.75, .earlyExitRange = 3});

    base.waitUntilDone();

    std::cout << "First swing at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.moveToPose(mogoPose.x, mogoPose.y, mogoPose.theta, 1000, {.forwards = false});

    base.waitUntil(8);

    closeClamp(true);

    global::overrideIntakeState = false;

    pros::delay(250);

    std::cout << "Got the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.turnToPoint(thirdRing.x, thirdRing.y, 1000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(thirdRing.x, thirdRing.y, 1000);

    // base.swingToPoint(-66, 64, lemlib::DriveSide::LEFT, 1000);

    base.swingToPoint(corner.x, corner.y, corner.lockedSide, 1000);

    base.moveToPoint(corner.x, corner.y, 3000);

    base.waitUntilDone();

    pros::delay(500);

    base.moveToPoint(reposition.x, reposition.y, 1000);

    // base.turnToPoint(-53, 36.2, 1000);

    // base.moveToPoint(-53, 36.2, 3000);

    base.turnToPoint(-48, 0, 1000);

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(sixthRing.x, sixthRing.y, 3000);

    base.waitUntil(40);

    intakePiston.set_value(true);

    base.swingToPoint(allianceStakeHeading.x, allianceStakeHeading.y, allianceStakeHeading.lockedSide, 1000);

    base.moveToPoint(allianceStake.x, allianceStake.y, 1000);
}



void soloAutonomousRoutine(bool blueSide) {
    int32_t startTime = pros::millis();

    coordinateInfo::Pose startingPosition(-51.3, -24, 270);
    coordinateInfo::Pose posMogo(-30, -24, 270);
    coordinateInfo::Point secondRing(-24, -48);
    coordinateInfo::Pose repositionForNeg1(-40.4, 23.0, 167);
    coordinateInfo::Point repositionForNeg2(-38.9, 13.9);
    coordinateInfo::Point negMogoHeading(-24, 24);
    coordinateInfo::Pose negMogo(-31.8, 18.3, 236);
    coordinateInfo::Point thirdRing(-24, 48);
    coordinateInfo::Pose ladder(-24, 9.3, 180);

    if (blueSide) {
        startingPosition.reflect();
        posMogo.reflect();
        secondRing.reflect();
        repositionForNeg1.reflect();
        repositionForNeg2.reflect();
        negMogoHeading.reflect();
        negMogo.reflect();
        thirdRing.reflect();
        ladder.reflect();
    }

    // See SG-1
    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPoint(posMogo.x, posMogo.y, 1500, {.forwards = false});

    base.waitUntil(19);

    closeClamp(true);

    pros::delay(250);

    std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    
    base.turnToPoint(secondRing.x, secondRing.y, 1000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(secondRing.x, secondRing.y, 1500);

    base.waitUntilDone();

    std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    

    base.moveToPose(repositionForNeg1.x, repositionForNeg1.y, repositionForNeg1.theta, 3500, {.forwards = false});

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Resting;

    base.moveToPoint(repositionForNeg2.x, repositionForNeg2.y, 1000);

    base.waitUntilDone();

    global::findNextDown = true;

    closeClamp(false);

    std::cout << "Done repositionining at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.turnToPoint(negMogoHeading.x, negMogoHeading.y, 1000, {.forwards = false}); // A heading of 236

    base.waitUntilDone();

    std::cout << base.getPose().theta << std::endl;

    base.moveToPoint(negMogo.x, negMogo.y, 2500, {.forwards = false});

    base.waitUntil(32);

    closeClamp(true);

    pros::delay(250);

    base.waitUntilDone();

    std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    // Grab another ring

    global::intakeState = states::intakeStates::Mogo;

    base.turnToPoint(thirdRing.x, thirdRing.y, 1000);

    base.moveToPoint(thirdRing.x, thirdRing.y, 1500);

    base.waitUntilDone();

    std::cout << "Done getting the fourth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.moveToPose(ladder.x, ladder.y, ladder.theta, 2000, {.minSpeed = 63.5});

    std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
}



void autonomous() {
    global::runningAuton = false;
    global::runningAuton = true;
    global::runningOpControl = false;



    /*
    if (!global::initializedBrainCoords) {
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

        global::initializedBrainCoords = true;
    }
    */
	setBrainImage();




    // optical.set_led_pwm(100);



    std::cout << "In autonomous" << std::endl;

    int32_t startTime = pros::millis();



    if (global::autonSelected == states::autonStates::RedPositiveCorner)
        positiveCornerRoutine(false);  
    else if (global::autonSelected == states::autonStates::RedNegativeCorner) {
        safeNegativeCornerRoutine(false);
    } else if (global::autonSelected == states::autonStates::RedSoloAWP) {
        soloAutonomousRoutine(false);
        // base.setPose(0,0,0);
        // base.moveToPoint(0, 12, 1000);
    } else if (global::autonSelected == states::autonStates::BluePositiveCorner) {
        // positiveCornerRoutine(true);  
        goalRushRoutine(true);
    } else if (global::autonSelected == states::autonStates::BlueNegativeCorner) {
        safeNegativeCornerRoutine(true);
    } else if (global::autonSelected == states::autonStates::BlueSoloAWP) {
        // See SG-1
        base.setPose(51.3, -24, 90);

        base.moveToPose(24, -24, 90, 1500, {.forwards = false, .minSpeed = 31.75});

        base.waitUntil(16);

        closeClamp(true);

        pros::delay(250);

        std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        
        base.turnToPoint(24, -48, 1000);

        base.waitUntilDone();

       global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(24, -48, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(41.4, 21.9, 194, 3000, {.forwards = false});

        base.waitUntilDone();

       global::intakeState = states::intakeStates::Resting;

        global::findNextDown = true;

        closeClamp(false);

        std::cout << "Done getting the third ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.turnToPoint(38.9, 13.9, 1000, {.forwards = false});

        base.moveToPose(38.9, 13.9, 194, 1000);

        base.turnToPoint(24, 24, 1000, {.forwards = false}); // A heading of 236

        base.waitUntilDone();

        std::cout << base.getPose().theta << std::endl;

        base.moveToPose(31.8, 18.3, 124, 2500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(250);

        base.waitUntilDone();

        std::cout << "Done getting the second mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

       global::intakeState = states::intakeStates::Mogo;

        base.turnToPoint(24.0, 48.0, 1000);

        base.moveToPoint(24.0, 48.0, 1500);

        base.waitUntilDone();

        std::cout << "Done getting the fourth ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(24, 9.3, 180, 2000, {.minSpeed = 63.5});

        std::cout << "Touching the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    } else if (global::autonSelected == states::autonStates::Skills) {
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

       global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-31.0, -22.6, 1500, {.minSpeed = 76.2, .earlyExitRange = 3});

        base.turnToHeading(180, 2000);

        base.waitUntilDone();

        std::cout << "Done turning at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;


        base.moveToPose(-31, -41.5, 180, 1500, {.minSpeed = 35, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "Done grabbing rings two and three at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // global::findNextDown = true;

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

       global::intakeState = states::intakeStates::Resting;

        global::findNextDown = true;

        closeClamp(false);

        pros::delay(250);

        std::cout << "Dropped the goal of in the corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Go to center and grab a ring

        // base.moveToPose(-36, -48, 0, 1500, {.minSpeed = 63.5, .earlyExitRange = 3});

        base.swingToPoint(0, 0, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .minSpeed = 83, .earlyExitRange = 5});

        global::storeRing = true;

        base.moveToPose(0, 0, 45, 3000);

        base.waitUntilDone();

        std::cout << "At center at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another ring

        base.swingToPoint(-16.9, 16.9, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 44.45, .earlyExitRange = 3});

        base.waitUntilDone();

        global::overrideIntakeState = true;

        intake.move_velocity(600);

        base.moveToPose(-24, 24, 315, 1500);

        base.waitUntilDone();

        global::overrideIntakeState = false;

        std::cout << "Grabbed rings seven and eight at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab another goal

        base.turnToHeading(90, 1000);

        base.moveToPose(-37.3, 23.6, 90, 3500, {.forwards = false});

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(500);

        std::cout << "Grabbed second goal at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        // Grab a ring

       global::intakeState = states::intakeStates::Mogo;

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

       global::intakeState = states::intakeStates::Resting;

        global::findNextDown = true;

        closeClamp(false);

        pros::delay(250);

        std::cout << "Dropped second goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;









        /*

        // Grab another ring

        base.moveToPose(0, 40, 125, 2500, {.minSpeed = 88.9, .earlyExitRange = 3});

        base.waitUntilDone();
        
        global::overrideIntakeState = true;

        intakePre.move_velocity(600);

        base.moveToPose(24, 24, 125, 1000, {.minSpeed = 88.9, .earlyExitRange = 3});

        // Shove a goal into a corner

        base.waitUntilDone();

        global::overrideIntakeState = false;

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

       global::intakeState = states::intakeStates::Mogo;

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

       global::intakeState = states::intakeStates::Resting;

        global::findNextDown = true;

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

        global::findNextDown = true;
        
        pros::delay(250);

        std::cout << "Dropped fourth goal in corner at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPose(59.0, -13.8, 1, 4000);

        base.waitUntilDone();

        std::cout << "Done with skills at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    } else if (global::autonSelected == states::autonStates::None) {
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

       global::intakeState = states::intakeStates::Mogo;

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

        /////


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

    // optical.set_led_pwm(0);

    global::runningAuton = false;
}