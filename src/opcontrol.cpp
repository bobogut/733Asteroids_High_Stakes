// Relevant PROS library are used for controller inputs, pneumatic control, and checking motor temperature (embeded in main.h). Other relevant header files are included for functions
// important to driver control.
#include "main.h"

#include "ports.h"
#include "pros/misc.h"
#include "robot_functions.h"
#include "states.h"



void opcontrol() {
    intakeState = INTAKERESTING;

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

    bool reverse = false; // Whether or not the controllers for the chassis should be reversed;

    if (autonSelected == SKILLS)
        reverse = true;



    while (true) {
        optical.set_led_pwm(100); // Turns on the LED in the optical sensor to ensure that lighting on the ring is consistent



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