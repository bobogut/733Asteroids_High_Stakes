// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include "main.h"

#include "ports.h"
#include "robot_functions.h"
#include "states.h"



void opcontrol() {

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;

    if (autonSelected == skills)
        reverse = true;



    while (true) {
        optical.set_led_pwm(100);



        if (leftMotors.is_over_temp_all()[0] || leftMotors.is_over_temp_all()[1] || leftMotors.is_over_temp_all()[2])
            controller.rumble("- _ _");

        if (rightMotors.is_over_temp_all()[0] || rightMotors.is_over_temp_all()[1] || rightMotors.is_over_temp_all()[2])
            controller.rumble("_ - -");



        updateIntakeStates();
        


        handlePneumatics();

        std::string clampOpen;

        if (mogoClamp.is_extended())
            clampOpen = "Closed";
        else
            clampOpen = "Opened";

        controller.set_text(0, 0, clampOpen); // To inform the driver whether or not the clamp is opened or closed when they cannot see it clearly


        
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) // When R2 is pressed toggle the reverse state
            reverse = !reverse;

        handleBase(reverse);



        pros::delay(5); // A delay for systems to not get overwhelmed and run properly
    }

    optical.set_led_pwm(0);
}