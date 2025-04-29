#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "main.h"

#include "my_includes/global_var.h"
#include "my_includes/ports.h"
#include "my_includes/states.h"
#include "my_includes/brain_screen.h"
#include "my_includes/CoordinateInfo.h"
#include "pros/motors.h"
#include <cstdint>



void closeClamp(bool close) {
    // I just created this so that I do not get confused when opening and closing the clamp
    if (close)
        mogoClamp.extend();
    else
        mogoClamp.retract();
}







void positiveCornerRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-51.3, -24, 270);
    lemlib::Pose safeMogo(-24, -24, 270);
    MyPoint secondRing(-24, -48);
    MyPoint ladder(-24, -9.3);

    if (blueSide) {
        startingPosition.reflect();
        safeMogo.reflect();
        secondRing.reflect();
        ladder.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    std::cout << "Starting: " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;


    base.moveToPose(safeMogo.x, safeMogo.y, safeMogo.theta, 1500, {.forwards = false});

    std::cout << "Mogo: " << safeMogo.x << ", y " << safeMogo.y << ", theta " << safeMogo.theta << std::endl;

    base.waitUntil(27);

    closeClamp(true);

    pros::delay(250);

    std::cout << "Done getting the first mogo at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
    
    base.turnToPoint(secondRing.x, secondRing.y, 1000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(secondRing.x, secondRing.y, 1500);

    base.waitUntilDone();

    std::cout << "Done getting the second ring at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

    base.turnToPoint(ladder.x, ladder.y, 1000);

    base.moveToPose(ladder.x, ladder.y, 0, 1500);

    base.waitUntil(24);

    global::armStatesQueue.push_back(states::armStates::WallStake);

    // std::cout << "Got to the ladder at time: " << pros::millis()- startTime << ", position is: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
}


void newGoalRush() {
    lemlib::Pose startingPosition(51.5, -59, 270);


    base.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    

    base.moveToPoint(11.8, -59, 1500, {.minSpeed = 63.5, .earlyExitRange = 1});


    base.waitUntil(34);

    doinker.extend();

    base.moveToPoint(36, -59, 3000, {.forwards = false, .maxSpeed = 38.1}); // 38.1 (30%) - 63.5 (50%)

    base.turnToPoint(0, -60, 1000, {.forwards = false});

    base.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    base.moveToPoint(0, -60, 1000, {.forwards = false, .earlyExitRange = 20});

    base.waitUntilDone();

    closeClamp(true);


    // base.setPose(-53, -24.6, 242.5);

    // base.moveToPose(19.8, -43.3, 237, 3000);

    /*

    doinker.retract();

    base.turnToPoint(24, 48, 750);

    base.moveToPoint(24, 48, 750);

    global::intakeState = states::intakeStates::FirstStage;

    base.turnToPoint(12, -59, 1000, {.forwards = false});
    */
}

void goalRushRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-51.5, -59, 90);
    MyPoint prepTurn(-18.8, -36.2);
    MyPoint grabGoal(-14.9, -41.1);

    if (blueSide) {
        startingPosition.reflect();
        prepTurn.reflect();
        grabGoal.reflect();
        // thirdRing.reflect();
        // preload.reflect();
    }

    base.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    global::intakeState = states::intakeStates::StoreRing;

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

void elimPositiveCornerRoutine(bool blueSide) {
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-58.2, -16.2, 315.3);
    MyPoint allianceStake(-61.0, -13.4);
    MyPoint repositionOne(-48, -24);
    MyPoint mogo(-18, -24);
    MyPoint secondRing(-28, -48);


    // base.setPose(-60.5, -14.4,315.3);

	// base.setPose(-58.2, -16.2,315.3);


    // -69, -48, 191

    // - 63, 44

    MyPoint reposition(-62, -44);

    MyPoint corner(-63.1, -53);

    MySwingTheta sweep(150, lemlib::DriveSide::RIGHT);

    lemlib::Pose collectRings(-40, -60, 90);


    // MyPoint thirdRing(-10, -51);
    // MyPoint ladder(-24, -6);

    if (blueSide) {
        startingPosition.reflect();
        allianceStake.reflect();
        repositionOne.reflect();
        mogo.reflect();
        secondRing.reflect();
        // thirdRing.reflect();
        // ladder.reflect();

        reposition.newPoint(-reposition.y, reposition.x);

        corner.newPoint(-corner.y, corner.x);

        sweep.newPoint(60, sweep.lockedSide);
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPoint(allianceStake.x, allianceStake.y, 750);

    base.waitUntilDone();

    global::armStatesQueue.push_back(states::armStates::AllianceStake);

    // while (global::armStatesQueue.size() != 0)
        // pros::delay(5);

    pros::delay(500);

    /*

    base.turnToPoint(,-24, 24, 1000);

    */

    base.moveToPoint(repositionOne.x, repositionOne.y, 750, {.forwards = false});

    base.turnToPoint(mogo.x, mogo.y, 750, {.forwards = false});

    base.moveToPoint(mogo.x, mogo.y, 1500, {.forwards = false});

    global::armStatesQueue.push_back(states::armStates::Resting);

    base.waitUntil(25);

    closeClamp(true);

    global::intakeState = states::intakeStates::Mogo;

    base.turnToPoint(secondRing.x, secondRing.y, 750);

    base.moveToPoint(secondRing.x, secondRing.y, 750);

    
    if (!blueSide) {

        /*
        base.turnToPoint(reposition.x, reposition.y, 750);


        base.moveToPoint(reposition.x, reposition.y, 1000);

        base.turnToPoint(corner.x, corner.y, 750);

        base.waitUntilDone();

        doinker.extend();

        // pros::delay(250);

        base.moveToPoint(corner.x, corner.y, 1000);

        base.swingToHeading(sweep.theta, sweep.lockedSide, 1000, {.minSpeed = 37.5, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "Time before sweep is " << pros::millis() - startTime << std::endl;


        base.turnToHeading(55, 750);

        base.waitUntilDone();

        std::cout << "Time after sweep is " << pros::millis() - startTime << std::endl;

        doinker.retract();



        base.moveToPose(collectRings.x, collectRings.y, collectRings.theta, 6000, {.maxSpeed = 57.2});

        // base.moveToPose(60, -40, 0, 6000, {.maxSpeed = 57.2});

        base.waitUntilDone();

        std::cout << "Time after motion is " << pros::millis() - startTime << std::endl;
        */


        /*
        base.turnToPoint(-57.4, -49.2, 750);

        base.moveToPoint(-57.4, -49.2, 2000, {.minSpeed = 37.5, .earlyExitRange = 3});

        base.moveToPose(-57.4, -49.2, 225, 2000, {.lead = 0.1, .maxSpeed = 37.5});

        */

        base.turnToPoint(-55, -55, 1000);

        base.moveToPoint(-55, -55, 3000);

        base.turnToPoint(-72, -72, 1000);


    } else {


        base.turnToPoint(44, -62, 750);

        base.moveToPoint(44, -62, 1000);

        base.turnToPoint(53, -63.1, 750);


        base.waitUntilDone();

        doinker.extend();

        pros::delay(250);

        base.moveToPoint(52, -61.1, 1000);

        base.waitUntilDone();

        std::cout << "Time before sweep is " << pros::millis() - startTime << std::endl;

        base.swingToHeading(60, lemlib::DriveSide::LEFT, 1000, {.minSpeed = 63.5, .earlyExitRange = 3});

        base.waitUntilDone();

        std::cout << "Time after sweep is " << pros::millis() - startTime << std::endl;

        doinker.retract();


        base.moveToPose(65, -30, 0, 6000, {.maxSpeed = 57.2});

        base.waitUntilDone();


        std::cout << "Time after motion is " << pros::millis() - startTime << std::endl;

        base.moveToPose(24, -48, 90, 2000, {.forwards = false});

    }

    






    

    

    

}

void safePositiveCornerRoutine(bool blueSide) {
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-58.2, -16.2, 315.3);
    MyPoint allianceStake(-61.0, -13.4);
    MyPoint repositionOne(-48, -24);
    MyPoint mogo(-18, -24);
    MyPoint secondRing(-28, -48);
    MyPoint ladder(-24, -6);

    if (blueSide) {
        startingPosition.reflect();
        allianceStake.reflect();
        repositionOne.reflect();
        mogo.reflect();
        secondRing.reflect();

        ladder.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    pros::delay(3000);

    base.moveToPoint(allianceStake.x, allianceStake.y, 750);

    base.waitUntilDone();

    global::armStatesQueue.push_back(states::armStates::AllianceStake);

    while (global::armStatesQueue.size() != 0)
        pros::delay(5);

    base.moveToPoint(repositionOne.x, repositionOne.y, 750, {.forwards = false});

    base.turnToPoint(mogo.x, mogo.y, 750, {.forwards = false});

    base.moveToPoint(mogo.x, mogo.y, 1500, {.forwards = false});

    global::armStatesQueue.push_back(states::armStates::Resting);

    base.waitUntil(24);

    closeClamp(true);

    global::intakeState = states::intakeStates::Mogo;

    base.turnToPoint(secondRing.x, secondRing.y, 750);

    base.moveToPoint(secondRing.x, secondRing.y, 750);


    base.turnToPoint(ladder.x, ladder.y, 750);

    base.moveToPoint(ladder.x, ladder.y, 3000, {.maxSpeed = 44.5});
}



void safeNegativeCornerRoutine(bool blueSide) {
    lemlib::Pose startingPosition(-58.2, 16.2, 224.7);
    MyPoint allianceStake(-61.0, 13.4);

    MyPoint repositionOne(-48, 24);
    lemlib::Pose mogo(-18, 24, 240);

    MyPoint repositionTwo(-17, 26);
    MySwingTheta ringPileTurn(0, lemlib::DriveSide::LEFT);
    lemlib::Pose ringPileMove(-14, 52, 0);

    MyPoint repositionThree(-14, 24);
    MyPoint ringStack(-24, 48);

    MyPoint ladder(-24, 6);


    if (blueSide) {
        startingPosition.reflect();
        allianceStake.reflect();

        repositionOne.reflect();
        mogo.reflect();

        repositionTwo.reflect();
        ringPileTurn.reflect();
        ringPileMove.reflect();

        repositionThree.reflect();
        ringStack.reflect();

        ladder.reflect();

    }

    std::cout << startingPosition.x << " " << startingPosition.y << std::endl;




    std::cout << repositionOne.x << " " << repositionOne.y << std::endl;

    // pros::delay(10000);

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPoint(allianceStake.x, allianceStake.y, 750);

    base.waitUntilDone();

    global::armStatesQueue.push_back(states::armStates::AllianceStake);

    while (global::armStatesQueue.size() != 0)
        pros::delay(5);

    base.moveToPoint(repositionOne.x, repositionOne.y, 750, {.forwards = false});

    base.turnToPoint(mogo.x, mogo.y, 750, {.forwards = false});

    base.moveToPoint(mogo.x, mogo.y, 1500, {.forwards = false});

    global::armStatesQueue.push_back(states::armStates::Resting);

    base.waitUntil(24);

    closeClamp(true);

    pros::delay(100);

    base.turnToPoint(repositionTwo.x, repositionTwo.y, 750);

    base.moveToPoint(repositionTwo.x, repositionTwo.y, 750, {.maxSpeed = 63.5});

    global::intakeState = states::intakeStates::Mogo;

    base.swingToHeading(ringPileTurn.theta, ringPileTurn.lockedSide, 750);

    base.moveToPoint(ringPileMove.x, ringPileMove.y, 2000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::FirstStage;

    base.moveToPoint(repositionThree.x, repositionThree.y, 2000, {.forwards = false});

    base.turnToPoint(ringStack.x, ringStack.y, 500);

    base.moveToPoint(ringStack.x, ringStack.y, 500);

    global::intakeState = states::intakeStates::Mogo;

    base.waitUntilDone();

    pros::delay(500);


    base.turnToPoint(ladder.x, ladder.y, 750);

    base.moveToPoint(ladder.x, ladder.y, 750, {.maxSpeed = 57.2});
}


// With alliance wallstake at beginning
void elimNegativeCornerOneRoutine(bool blueSide) {
    lemlib::Pose startingPosition(-58.2, 13.3, 217);
    MyPoint allianceStake(-60, 10.2);

    MyPoint repositionOne(-48, 24);
    lemlib::Pose mogo(-18, 24, 240);

    MyPoint repositionTwo(-16, 36);
    MySwingTheta ringPileTurn(5, lemlib::DriveSide::LEFT);
    lemlib::Pose ringPileMove(-14, 52, 0);

    MyPoint repositionThree(-14, 24);
    MyPoint ringStack(-24, 48);

    MyPoint allianceRing(-48, 0);
    MyPoint positiveCorner(-72, -72);

    if (blueSide) {
        startingPosition.reflect();
        allianceStake.reflect();

        repositionOne.reflect();
        mogo.reflect();

        repositionTwo.reflect();
        ringPileTurn.reflect();
        ringPileMove.reflect();

        repositionThree.reflect();
        ringStack.reflect();

        allianceRing.reflect();
        positiveCorner.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPoint(allianceStake.x, allianceStake.y, 750);

    base.waitUntilDone();

    global::armStatesQueue.push_back(states::armStates::AllianceStake);

    while (global::armStatesQueue.size() != 0)
        pros::delay(2);

    base.moveToPoint(repositionOne.x, repositionOne.y, 750, {.forwards = false});

    base.turnToPoint(mogo.x, mogo.y, 750, {.forwards = false});

    base.moveToPoint(mogo.x, mogo.y, 1500, {.forwards = false});

    global::armStatesQueue.push_back(states::armStates::Resting);

    base.waitUntil(24);

    closeClamp(true);

    pros::delay(100);

    base.turnToPoint(repositionTwo.x, repositionTwo.y, 750);

    base.moveToPoint(repositionTwo.x, repositionTwo.y, 750, {.maxSpeed = 63.5});

    global::intakeState = states::intakeStates::Mogo;

    base.swingToHeading(ringPileTurn.theta, ringPileTurn.lockedSide, 750);

    base.moveToPoint(ringPileMove.x, ringPileMove.y, 2000);

    base.waitUntilDone();

    global::intakeState = states::intakeStates::Resting;

    base.moveToPoint(repositionThree.x, repositionThree.y, 2000, {.forwards = false});

    base.turnToPoint(ringStack.x, ringStack.y, 500);

    base.moveToPoint(ringStack.x, ringStack.y, 500);

    global::intakeState = states::intakeStates::Mogo;

    base.waitUntilDone();

    pros::delay(500);

    base.turnToPoint(allianceRing.x, allianceRing.y, 750);

    base.moveToPoint(allianceRing.x, allianceRing.y, 2000);

    base.waitUntil(18);

    global::intakeState = states::intakeStates::FirstStage;

    base.turnToPoint(0, 0, 750);

    global::intakeState = states::intakeStates::Reverse;

    pros::delay(2000);

    base.turnToPoint(positiveCorner.x, positiveCorner.y, 750);

    base.moveToPoint(positiveCorner.x, positiveCorner.y, 750);

    global::intakeState = states::intakeStates::Mogo;
}

// With alliance wallstake at end
void elimNegativeCornerTwoRoutine(bool blueSide) {
    lemlib::Pose startingPosition(-51.3, -24, 270);
    MyPoint allianceStake(-60, 10.2);
    MyPoint repositionOne(-48, 24);
    lemlib::Pose mogo(-18, 24, 240);
    MySwingPoint ringPileTurn(-14, 72, lemlib::DriveSide::LEFT);
    MyPoint ringPileMoveOne(0, 48);
    MyPoint repositionThree(-16, 36);

    lemlib::Pose ringPileMove(-14, 52, 0);
    lemlib::Pose ringPileMoveTwo(-10, 50, 0);

    MyPoint repositionTwo(-14, 24);
    MyPoint ringStack(-24, 48);
    MyPoint ladder(-24, 8);

    if (blueSide) {
        startingPosition.reflect();
        allianceStake.reflect();
        repositionOne.reflect();
        mogo.reflect();
        ringPileTurn.reflect();
        ringPileMove.reflect();
        repositionTwo.reflect();
        ringStack.reflect();
        ladder.reflect();
    }

    base.setPose(startingPosition.x, startingPosition.y, startingPosition.theta);

    base.moveToPose(mogo.x, mogo.y, mogo.theta, 1500, {.forwards = false});

    base.waitUntilDone();

    closeClamp(true);

    pros::delay(250);

    base.turnToPoint(repositionOne.x, repositionTwo.y, 1000);

    // base.moveToPose(thirdRing.x, thirdRing.y, 0, 1000);

    // base.moveToPoint(-10.7, 39.8, 1000);

    // base.swingToPoint(-8.0, 50.0, lemlib::DriveSide::LEFT, 1000);

    // base.moveToPoint(-8.0, 44.0, 1000);

    base.turnToPoint(repositionTwo.x, repositionTwo.y, 1000);

    base.moveToPoint(repositionTwo.x, repositionTwo.y, 2000, {.minSpeed = 25.4, .earlyExitRange = 2});

    // base.swingToPoint(fourthRing.x, fourthRing.y, fourthRing.lockedSide, 1000);

    // base.moveToPoint(fourthRing.x, fourthRing.y, 1500, {.minSpeed = 63.5, .earlyExitRange = 4});

    // base.turnToPoint(-48, 0, 1000);

    // base.moveToPoint(-48, 0, 3000);
}



void ringRushRoutine(bool blueSide) {
    // All coordiantes are by default meant for the red side
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-51.6, 36.2, 90);
    MyPoint prepTurn(-11.0, 36.2);
    MyPoint grabGoal(-14.9, 41.1);
    MySwingTheta wall(0, lemlib::DriveSide::LEFT);
    float secondRingY = 45;
    MySwingTheta mogoHeading(10, lemlib::DriveSide::RIGHT);
    lemlib::Pose mogoPose(-18, 30, 10);
    MyPoint thirdRing(-24, 40);
    MySwingPoint corner(-61, 62, lemlib::DriveSide::LEFT);
    MyPoint reposition(-54, 57.8);
    MyPoint sixthRing(-48.9, 8.7);
    MySwingPoint allianceStakeHeading(-72.0, 0.0, lemlib::DriveSide::RIGHT);
    MyPoint allianceStake(-58.0, 0.0);

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

    global::intakeState = states::intakeStates::StoreRing;
    
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

    base.swingToPoint(allianceStakeHeading.x, allianceStakeHeading.y, allianceStakeHeading.lockedSide, 1000);

    base.moveToPoint(allianceStake.x, allianceStake.y, 1000);
}



void soloAutonomousRoutine(bool blueSide) {
    int32_t startTime = pros::millis();

    lemlib::Pose startingPosition(-51.3, -24, 270);
    lemlib::Pose posMogo(-30, -24, 270);
    MyPoint secondRing(-24, -48);
    lemlib::Pose repositionForNeg1(-40.4, 23.0, 167);
    MyPoint repositionForNeg2(-38.9, 13.9);
    MyPoint negMogoHeading(-24, 24);
    lemlib::Pose negMogo(-31.8, 18.3, 236);
    MyPoint thirdRing(-24, 48);
    lemlib::Pose ladder(-24, 9.3, 180);

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








void waitForRing(void* count) {
    while (global::ringsDetected / 2 != (int)count)
        pros::delay(5);

    global::intakeState = states::intakeStates::Arm;
}



void fillMogoAndWall(bool firstHalf) {
    int32_t tempTime = 0;

    MyPoint firstRingMogo(-26, -24);
    MyPoint secondRingMogo(0, -60);
    // MyPoint firstRingArm(48, -62);

    // MyPoint repositionWallStake(0, -56);
    // int wallStakeTurn = 180;
    // MyPoint wallStake(0, -80);

    MyPoint thirdRingMogo(-28, -48);
    MyPose fiveRingMogo(-60, -48, 270);
    MyPose repositionRing(-52, -48, 270);
    MyPoint sixRingMogo(-48, -62);

    MySwingPoint cornerReposition(-72, -66, lemlib::DriveSide::RIGHT);

    if (!firstHalf) {
        firstRingMogo.reflectSkills();
        secondRingMogo.reflectSkills();
        // firstRingArm.reflectSkills();

        // repositionWallStake.reflectSkills();
        // wallStakeTurn = 0;
        // wallStake.reflectSkills();

        thirdRingMogo.reflectSkills();
        fiveRingMogo.reflectSkills();
        repositionRing.reflectSkills();
        sixRingMogo.reflectSkills();

        cornerReposition.reflectSkills();
    }


    // Grab ring

    base.turnToPoint(firstRingMogo.x, firstRingMogo.y, 750, {.maxSpeed = 76});

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(firstRingMogo.x, firstRingMogo.y, 1000, {.minSpeed = 76.2, .earlyExitRange = 8});

    base.waitUntilDone();

    std::cout << "Scored one ring on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;




    // Travel across the field, grab another ring and store one

    base.turnToPoint(secondRingMogo.x, secondRingMogo.y, 500); // , {.minSpeed = 114, .earlyExitRange = 3}

    base.moveToPoint(secondRingMogo.x, secondRingMogo.y, 1500, {.maxSpeed = 71.4, .minSpeed = 12.7, .earlyExitRange = 3});

    // base.turnToPoint(firstRingArm.x, firstRingArm.y, 500); // , {.minSpeed = 114, .earlyExitRange = 3}

    // base.moveToPoint(firstRingArm.x, firstRingArm.y, 2000, {.maxSpeed = 71.4, .minSpeed = 6.35, .earlyExitRange = 4});

    // pros::Task storeArm(waitForRing, (void*) 3); // Create a task for moving the intake so that the rest of the code will not 


    base.waitUntilDone();

    std::cout << "Scored two ring on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

    // pros::delay(250);

    // base.moveToPoint(secondRingMogo.x, secondRingMogo.y, 1000, {.forwards = false});


    // base.turnToPoint(repositionWallStake.x, repositionWallStake.y, 750);

    // base.moveToPoint(repositionWallStake.x, repositionWallStake.y, 1000);

    // base.waitUntilDone();

    // std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


    // global::intakeState = states::intakeStates::FirstStageReverse;

    // global::armStatesQueue.push_back(states::armStates::PrimeOne);

    // Score on wall stake

    /*
    if (global::armState != states::armStates::PrimeOne) {
        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        global::intakeState = states::intakeStates::Mogo;
    }

    base.waitUntilDone();

    base.turnToHeading(wallStakeTurn, 1000);

    base.waitUntilDone();

    std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

    base.moveToPoint(wallStake.x, wallStake.y, 750);

    base.waitUntilDone();

    std::cout << "At wallstake at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;





    global::intakeState = states::intakeStates::Arm;

    tempTime = pros::millis();

    while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime <= 1000)
        pros::delay(5);

    global::armStatesQueue.push_back(states::armStates::WallStake);

    global::armStatesQueue.push_back(states::armStates::PrimeOne);

    while (global::armStatesQueue.size() != 0)
        pros::delay(5);
    global::intakeState = states::intakeStates::Mogo;

    tempTime = pros::millis();

    while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime <= 1000)
        pros::delay(5);

    global::armStatesQueue.push_back(states::armStates::WallStake);

    global::armStatesQueue.push_back(states::armStates::Resting);

    while (global::armStatesQueue.size() != 0)
        pros::delay(5);


    // Score on mogo and corner


    base.moveToPoint(repositionWallStake.x, repositionWallStake.y, 1000, {.forwards = false});

*/


    base.turnToPoint(thirdRingMogo.x, thirdRingMogo.y, 2000);

    global::intakeState = states::intakeStates::Mogo;

    base.moveToPoint(thirdRingMogo.x, thirdRingMogo.y, 3000, {.maxSpeed = 63.5, .minSpeed = 12.7, .earlyExitRange = 6});

    base.waitUntilDone();

    std::cout << "Scored three rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

    base.moveToPose(fiveRingMogo.x, fiveRingMogo.y, fiveRingMogo.theta, 3000, {.lead = 0.3, .maxSpeed = 63.5, .earlyExitRange = 1});

    base.waitUntilDone();

    std::cout << "Scored five rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


    // base.setPose(-63, -48, 270);

    base.moveToPose(repositionRing.x, repositionRing.y, repositionRing.theta, 750, {.forwards = false});

    base.turnToPoint(sixRingMogo.x, sixRingMogo.y, 750);

    base.moveToPoint(sixRingMogo.x, sixRingMogo.y, 750, {.minSpeed = 6.35, .earlyExitRange = 2});

    base.waitUntilDone();

    std::cout << "Scored six rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;













    // Deposit mogo in corner   

    base.swingToPoint(cornerReposition.x, cornerReposition.y, cornerReposition.lockedSide, 1000, {.forwards = false});

    base.waitUntilDone();


    base.moveToPoint(cornerReposition.x, cornerReposition.y, 1000, {.forwards = false, .maxSpeed = 44.5});

    base.waitUntilDone();

    closeClamp(false);
}







void autonomous() {
    global::runningAuton = false;
    global::runningAuton = true;
    global::runningOpControl = false;

    global::ranAuton = true;

    


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
	// setBrainImage();




    optical.set_led_pwm(100);



    std::cout << "In autonomous" << std::endl;

    int32_t startTime = pros::millis();

    int32_t tempTime = 0;



    
    if (global::autonSelected == states::autonStates::RedPositiveCorner)
        safePositiveCornerRoutine(false);  
    else if (global::autonSelected == states::autonStates::RedNegativeCorner) {
        safeNegativeCornerRoutine(false);
    } else if (global::autonSelected == states::autonStates::RedSoloAWP) {
        safeNegativeCornerRoutine(false);
    } else if (global::autonSelected == states::autonStates::BluePositiveCorner) {
        // safePositiveCornerRoutine(true);  
        // goalRushRoutine(true);
        newGoalRush();
    } else if (global::autonSelected == states::autonStates::BlueNegativeCorner) {
        safeNegativeCornerRoutine(true);
    } else if (global::autonSelected == states::autonStates::BlueSoloAWP) {
        safeNegativeCornerRoutine(true);
    } else if (global::autonSelected == states::autonStates::Skills) {

        /*
        pros::Task storeArm(waitForRing, (void*) 3); // Create a task for moving the intake so that the rest of the code will not 

        global::intakeState = states::intakeStates::Mogo;

        while(true)
            pros::delay(5);
        */
        





        // base.setPose(-60, -10.9, 323);

        base.setPose(-60.5, -14.4,315.3);


        // Score alliance wallstake


        global::armStatesQueue.push_back(states::armStates::AllianceStake);

        // while (global::armStatesQueue.size() != 0)
            // pros::delay(5);

        pros::delay(500);


        // Grab goal

        base.moveToPoint(-53.7, -19.2, 750, {.forwards = false});

        base.waitUntil(5);

        global::armStatesQueue.push_back(states::armStates::Resting);

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(100);


        
        fillMogoAndWall(true);

        base.moveToPose(-48, 0, 0, 6000);


        base.turnToPoint(-48, -24, 750);

        base.moveToPoint(-48, -24, 2000);

        base.waitUntil(20);

        closeClamp(true);

        pros::delay(100);

        fillMogoAndWall(false);


        /*
        // Grab ring

        base.turnToPoint(-26, -24, 750, {.maxSpeed = 76});

        global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-26, -24, 1000, {.minSpeed = 76.2, .earlyExitRange = 8});

        base.waitUntil(24);

        base.waitUntilDone();

        std::cout << "Scored one ring on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;




        // Travel across the field, grab another ring and store one

        base.turnToPoint(48, -64, 500); // , {.minSpeed = 114, .earlyExitRange = 3}

        base.moveToPoint(48, -64, 2000, {.maxSpeed = 71.4, .minSpeed = 6.35, .earlyExitRange = 16});

        pros::Task storeArm(waitForRing, (void*) 3); // Create a task for moving the intake so that the rest of the code will not 


        base.waitUntilDone();

        std::cout << "Scored two ring on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        // pros::delay(250);

        base.turnToPoint(0, -56, 750, {.forwards = false});



        base.moveToPoint(0, -56, 1000, {.forwards = false, .maxSpeed = 12.7, .earlyExitRange = 8});

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // global::intakeState = states::intakeStates::FirstStageReverse;

        // global::armStatesQueue.push_back(states::armStates::PrimeOne);

        // Score on wall stake

        base.turnToPoint(0, -56, 750);


        base.moveToPoint(-5, -56, 2000, {.maxSpeed = 127});


        if (global::armState != states::armStates::PrimeOne) {
            global::armStatesQueue.push_back(states::armStates::PrimeOne);

            global::intakeState = states::intakeStates::Mogo;
        }

        base.waitUntilDone();

        base.turnToHeading(180, 1000);

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        // base.turnToPoint(0, -72, 1000);

        base.moveToPoint(0, -80, 750);




        base.waitUntilDone();

        std::cout << "At wallstake at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        global::intakeState = states::intakeStates::Arm;

        tempTime = pros::millis();

        while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime < 2000)
            pros::delay(5);

        global::armStatesQueue.push_back(states::armStates::WallStake);

        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        while (global::armStatesQueue.size() != 0)
            pros::delay(5);

        global::intakeState = states::intakeStates::Mogo;

        tempTime = pros::millis();

        while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime < 2000)
            pros::delay(5);

        global::armStatesQueue.push_back(states::armStates::WallStake);

        global::armStatesQueue.push_back(states::armStates::Resting);

        while (global::armStatesQueue.size() != 0)
            pros::delay(5);



        // Score on mogo and corner


        base.moveToPoint(0, -48, 1000, {.forwards = false});

        base.turnToPoint(-28, -48, 2000);

        global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-28, -48, 3000, {.maxSpeed = 63.5, .minSpeed = 12.7, .earlyExitRange = 6});

        base.waitUntilDone();

        std::cout << "Scored three rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        base.moveToPose(-60, -48, 270, 3000, {.lead = 0.3, .maxSpeed = 63.5, .earlyExitRange = 1});

        base.waitUntilDone();

        std::cout << "Scored five rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // base.setPose(-63, -48, 270);

        base.moveToPose(-52, -48, 270, 750, {.forwards = false});

        base.turnToPoint(-48, -62, 750);

        base.moveToPoint(-48, -62, 750, {.minSpeed = 6.35, .earlyExitRange = 2});

        base.waitUntilDone();

        std::cout << "Scored six rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;













        // Deposit mogo in corner   

        base.swingToPoint(-72, -66, lemlib::DriveSide::RIGHT, 1000, {.forwards = false});

        base.waitUntilDone();


        base.moveToPoint(-72, -66, 1000, {.forwards = false, .maxSpeed = 44.5});

        base.waitUntilDone();

        closeClamp(false);


        // base.moveToPoint(-60, -60, 750, {.forwards = false});





















        // Grab the second goal

        base.moveToPose(-48, -12, 0, 2000, {.lead = 0.3, .maxSpeed = 63.5});

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        base.turnToPoint(-48, 24, 750, {.forwards = false});



        base.moveToPoint(-48, 30, 2500, {.forwards = false, .maxSpeed = 63.5});

        base.waitUntil(36);

        closeClamp(true);

        base.waitUntilDone();

        pros::delay(100);

        std::cout << "Grabbed goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        

        // Score one ring

        base.turnToPoint(-26, 24, 1000, {.maxSpeed = 64});

        global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-26, 24, 1000, {.minSpeed = 76.2, .earlyExitRange = 8});

        std::cout << "Scored one ring on goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;




        // Travel acros the field; score one ring and store one

        base.turnToPoint(48, 64, 500);

        base.moveToPoint(48, 64, 2000, {.maxSpeed = 71.4, .minSpeed = 6.35, .earlyExitRange = 16});

        base.waitUntil(60);

        global::intakeState = states::intakeStates::Arm;

        base.waitUntilDone();

        std::cout << "Scored two ring on goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        // pros::delay(250);




        base.turnToPoint(0, 56, 750, {.forwards = false});

        base.moveToPoint(0, 56, 1000, {.forwards = false, .maxSpeed = 12.7, .earlyExitRange = 8});

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;



        // Score on wallstake

        base.turnToPoint(0, 56, 750);
        

        base.moveToPoint(4, 56, 2000, {.maxSpeed = 127});

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // base.turnToHeading(4, 56, 1000);

        base.turnToHeading(0, 1000);

        base.moveToPoint(0, 80, 1000);




        base.waitUntilDone();

        std::cout << "At wallstake at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        global::intakeState = states::intakeStates::Mogo;

        tempTime = pros::millis();

        while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime < 2000)
            pros::delay(5);

        global::armStatesQueue.push_back(states::armStates::WallStake);

        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        while (global::armStatesQueue.size() != 0)
            pros::delay(5);

        global::intakeState = states::intakeStates::Mogo;

        tempTime = pros::millis();

        while (global::intakeState != states::intakeStates::FirstStage && pros::millis() - tempTime < 2000)
            pros::delay(5);

        global::armStatesQueue.push_back(states::armStates::WallStake);

        global::armStatesQueue.push_back(states::armStates::Resting);

        while (global::armStatesQueue.size() != 0)
            pros::delay(5);



        // Score on mogo and corner


        base.moveToPoint(0, 48, 1000, {.forwards = false});

        base.turnToPoint(-28, 48, 2000);

        global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-28, 48, 3000, {.maxSpeed = 63.5, .minSpeed = 12.7, .earlyExitRange = 6});

        base.waitUntilDone();

        std::cout << "Scored three rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        base.moveToPose(-60, 48, 270, 3000, {.lead = 0.3, .maxSpeed = 63.5, .earlyExitRange = 1});

        base.waitUntilDone();

        std::cout << "Scored five rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // base.setPose(-63, -48, 270);

        base.moveToPose(-52, 48, 270, 750, {.forwards = false});

        base.turnToPoint(-48, 62, 750);

        base.moveToPoint(-48, 62, 750, {.minSpeed = 6.35, .earlyExitRange = 2});

        base.waitUntilDone();

        std::cout << "Scored six rings on mogo at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;







        // Corner mogo

        base.swingToPoint(-72, 66, lemlib::DriveSide::LEFT, 1000, {.forwards = false});

        base.waitUntilDone();


        base.moveToPoint(-72, 66, 750, {.forwards = false});

        base.waitUntilDone();

        closeClamp(false);
        */








        /*


        // Store a ring in arm

        base.moveToPoint(48, 48, 2000);

        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        base.waitUntil(96);

        global::intakeState = states::intakeStates::Mogo;

        base.waitUntilDone();

        std::cout << "Stored ring at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // Grab blue goal and deposit in corner, store ring in first stage

        base.swingToPoint(60, 24, lemlib::DriveSide::RIGHT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});

        base.moveToPoint(60, 24, 750);

        base.waitUntil(20);

        closeClamp(true);

        base.waitUntilDone();

        std::cout << "Grabbed goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;




        base.turnToPoint(60, 48, 750);

        global::intakeState = states::intakeStates::FirstStage;

        base.moveToPose(58, 60, 0, 1500);

        doinker.extend();

        base.turnToPoint(60, 0, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});

        base.waitUntilDone();

        doinker.retract();

        closeClamp(false);

        pros::delay(100);

        std::cout << "Deposited goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;





        // Score on alliance wall stake

        base.moveToPoint(60, 0, 1500);

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        base.turnToPoint(72, 0, 750);

        base.moveToPoint(56, 0, 750, {.forwards = false});

        base.waitUntilDone();

        std::cout << "Grabbed goal at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        closeClamp(true);

        global::armStatesQueue.push_back(states::armStates::AllianceStake);

        while(global::armStatesQueue.size() != 0)
            pros::delay(5);






        // Fill third goal

        base.moveToPoint(40, 0, 750, {.forwards = false});

        base.turnToPoint(24, 24, 750);

        base.moveToPoint(24, 24, 750, {.minSpeed = 63.5, .earlyExitRange = 7});

        base.swingToPoint(0, 0, lemlib::DriveSide::LEFT, 750);

        base.moveToPoint(0, 0, 750, {.minSpeed = 44.5, .earlyExitRange = 4});

        base.swingToPoint(48, -48, lemlib::DriveSide::LEFT, 750);

        base.moveToPoint(48, -48, 750);

        base.swingToPoint(60, -48, lemlib::DriveSide::LEFT, 750);

        base.moveToPoint(60, -48, 750);

        base.turnToPoint(72, -72, 750, {.forwards = false});

        base.moveToPoint(72, -72, 750, {.forwards = false, .earlyExitRange = 4});

        base.waitUntilDone();

        closeClamp(false);
        */




        
        /*
        base.setPose(-60, -10.9, 323);

        global::armStatesQueue.push_back(states::armStates::AllianceStake);

        while (global::armStatesQueue.size() != 0)
            pros::delay(2);

        base.moveToPoint(-53.7, -19.2, 1000, {.forwards = false});

        base.waitUntil(5);

        global::armStatesQueue.push_back(states::armStates::Resting);

        base.waitUntilDone();

        closeClamp(true);

        pros::delay(100);

        base.turnToPoint(-26, -24, 1000);

        global::intakeState = states::intakeStates::Mogo;

        base.moveToPoint(-26, -24, 1000, {.minSpeed = 76.2, .earlyExitRange = 8});

        base.waitUntilDone();

        std::cout << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        base.turnToPoint(0, -60, 1000); // , {.minSpeed = 114, .earlyExitRange = 3}

        base.moveToPoint(0, -60, 2000, {.minSpeed = 38.1, .earlyExitRange = 6});

        base.waitUntilDone();

        std::cout << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        pros::delay(500);

        // base.setPose()

        base.moveToPoint(0, -58, 1000, {.forwards = false});

        base.waitUntilDone();

        std::cout << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;

        base.turnToPoint(-28, -48, 2000);

        base.moveToPoint(-28, -48, 3000, {.minSpeed = 50.8, .earlyExitRange = 1});

        base.waitUntilDone();

        std::cout << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // base.moveToPose(-60, -48, 270,  20000, {.maxSpeed = 88.9, .minSpeed = 38.1, .earlyExitRange = 1});

        base.moveToPoint(-71, -48, 5000, {.maxSpeed = 63.5, .earlyExitRange = 1});

        base.waitUntilDone();

        base.setPose(-63, -48, 270);

        std::cout << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        base.moveToPose(-52, -48, 270, 1000, {.forwards = false});

        // base.swingToPoint(-48, -60, lemlib::DriveSide::LEFT, 1000);

        base.swingToPoint(24, -26, lemlib::DriveSide::LEFT, 1000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});

        base.swingToPoint(19, -28.1, lemlib::DriveSide::RIGHT, 1000);

        base.waitUntilDone();

        pros::delay(2000);

        closeClamp(false);

        pros::delay(100);

        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        base.moveToPoint(24, -24, 3000); // , {.minSpeed = 76.2, .earlyExitRange = 2}

        // base.waitUntil(10);

        global::intakeState = states::intakeStates::Mogo;

        base.turnToPoint(24, -48, 1000);

        base.waitUntilDone();

        global::armState = states::armStates::PrimeTwo;

        global::intakeState = states::intakeStates::StoreRing;

        base.moveToPoint(22, -40, 1000); // , {.minSpeed = 63.5, .earlyExitRange = 2}

        base.swingToPoint(0, -62, lemlib::DriveSide::RIGHT, 1000); // , {.minSpeed = 63.5, .earlyExitRange = 5}

        base.moveToPose(0, -62, 180, 2000);

        base.waitUntilDone();

        base.setPose(0, -62, 180);

        global::armStatesQueue.push_back(states::armStates::WallStake);

        global::armStatesQueue.push_back(states::armStates::PrimeOne);

        while (global::armStatesQueue.size() != 0)
            pros::delay(2);


        global::armStatesQueue.push_back(states::armStates::WallStake);
        
        while (global::armStatesQueue.size() != 0)
            pros::delay(2);
        */
    } else if (global::autonSelected == states::autonStates::None) {






    
        /*

        base.setPose(33.9, -56.4, 116);




        base.turnToPoint(0, -48, 1000, {.forwards = false});

        // base.turnToHeading(104, 1000);  

        base.waitUntilDone();

        std::cout << "MY heading is " << base.getPose().theta << std::endl;

        base.moveToPoint(0, -48, 5000, {.forwards = false, .minSpeed = 38.1});

        base.waitUntilDone();

        std::cout << "Reposition at " << base.getPose().x << ", " << base.getPose().y << ", " << base.getPose().theta << std::endl;


        // global::intakeState = states::intakeStates::FirstStageReverse;

        // global::armStatesQueue.push_back(states::armStates::PrimeOne);

        // Score on wall stake

        // base.turnToPoint(0, -56, 750);


        // base.moveToPoint(-5, -56, 2000, {.maxSpeed = 127});

        // global::intakeState = states::intakeStates::Mogo;

        // base.waitUntil(24);


        // base.waitUntilDone();

        base.turnToHeading(180, 1000);

        */








        /*
        base.setPose(56.1, 31.9, 282); // 56.7, 37.9, 287

        global::intakeState = states::intakeStates::FirstStage;

        base.moveToPoint(10.3, 41.6, 2000, {.minSpeed = 63.5, .earlyExitRange = 6});

        base.moveToPoint(31.4, 37.2, 1000, {.forwards = false});

        // base.turnToHeading(0, 1000);

        base.waitUntilDone();

        global::intakeState = states::intakeStates::Reverse;
        */


        // base.setPose(24, 48, 270);


        /*
        base.setPose(0, 0, 0);

        base.turnToHeading(180, 10000);

        base.waitUntilDone();

        std::cout << "Base at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;
        */

        /*
        base.setPose(0, 0, 0);

        std::cout << "Base at: x " << base.getPose().x << ", y " << base.getPose().y << ", theta " << base.getPose().theta << std::endl;

        base.moveToPoint(0, 48, 5000);

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
