#include "main.h"

#include "lemlib/chassis/chassis.hpp"

#include "ports.h"
#include "pros/misc.h"
#include "robot_functions.h"
#include "states.h"



void initialAutonPosition() {
    if (autonSelected == redPositiveCorner) {
        base.setPose(-47.9, -30.5, 0);

        // base.moveToPose(-48.3, -28.3, 287, 1000);
    }
    else if (autonSelected == redNegativeCorner) {
        base.setPose(-52.9, 16.7, 0);

        base.moveToPose(-58.2, 12.5, 55, 1000, {.forwards = false});
    }
    else if (autonSelected == blueNegativeCorner) {
    }
    else if (autonSelected == bluePositiveCorner) {
    }
    else if (autonSelected == skills) {
    }
}

void opcontrol() {

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;

    if (autonSelected == skills) {
        reverse = true;
    }


    
    std::string clampOpen;



    while (true) {
        optical.set_led_pwm(100);

        if (!autonRan && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) // Before autonomous period begins and the B button is pressed
            initialAutonPosition();                                                                // Move the robot to its initial autonomous position


        
        handlePneumatics();


        
        updateIntakeStates();


        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When R2 is pressed toggle the reverse state
            reverse = !reverse;

        handleBase(reverse);



        if (mogoClamp.is_extended())
            clampOpen = "Closed";
        else
            clampOpen = "Opened";

        controller.set_text(0, 0, clampOpen);



        // std::cout << "Vertical is " << verticalRotationSensor.get_angle() << std::endl;
	    // std::cout << "Horizontal is " << horizontalRotationSensor.get_angle() << std::endl;

        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    optical.set_led_pwm(0);
}