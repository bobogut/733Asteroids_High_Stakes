// Relevant PROS library are used for tasks and checking motor temperature (embeded in main.h). Other relevant header files are included to declare the fuynction for out intake task and
// declare relevant motors to modify their attributes.
#include "main.h"

#include "auton_selector.h"
#include "ports.h"
#include "robot_functions.h"



void initialize() {
	optical.set_led_pwm(0); // Turn off the LED on the optical sensor to avoid draining too much from the battery



	base.calibrate(); // Calibrates the sensors for the base



	global::autonSelected = None;


	// Default intake states
	global::intakeState = INTAKERESTING;
	global::overrideIntakeState = false;
	


	// Reset the intake motor positions
	intakePre.tare_position();
	intakeHook.tare_position();

	std::cout << intakeHook.get_position() << std::endl;

	// Set the intake motors to coast (i.e. when a movement is over gradually slow down) to avoid burnout, should only add some minor drift
	intakePre.set_brake_mode(pros::v5::MotorBrake::coast);
	intakeHook.set_brake_mode(pros::v5::MotorBrake::coast);

	pros::task_t updateIntakeSpeed = pros::Task::create(handleIntake, "Update intake speed"); // Create a task for moving the intake so that the rest of the code will not 
																											  // potentially be interrupted



	// Same logic as before but for the base motors and tracking wheels
	leftMotors.tare_position_all();
	rightMotors.tare_position_all();

	leftMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);
	rightMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);

	verticalRotationSensor.reset_position();
	horizontalRotationSensor.reset_position();

	// Useful for checking when our base is overheating so that we can let it rest
	if (leftMotors.is_over_temp_all()[0] || leftMotors.is_over_temp_all()[1] || leftMotors.is_over_temp_all()[2])
		controller.rumble("- _ _");

	if (rightMotors.is_over_temp_all()[0] || rightMotors.is_over_temp_all()[1] || rightMotors.is_over_temp_all()[2])
		controller.rumble("_ - -");

	if (intakePre.is_over_temp() || intakeHook.is_over_temp())
		controller.rumble("_ - _");
}



void disabled() {}



void competition_initialize() {
	autonSelectorInitialization(); // Creates buttons on the brain screen for selecting autonomous routines
}