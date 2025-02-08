// Relevant PROS library are used for tasks and checking motor temperature (embeded in main.h). Other relevant header files are included to declare the fuynction for out intake task and
// declare relevant motors to modify their attributes.
#include "main.h"

#include "my_includes/auton_selector.h"
#include "my_includes/global_var.h"
#include "my_includes/ports.h"
#include "my_includes/robot_functions.h"
#include "my_includes/states.h"



void initialize() {
	pros::task_t writeToFile = pros::Task::create( storeCoordinates, "Store cooridnates in SD card");



	optical.set_led_pwm(0); // Turn off the LED on the optical sensor to avoid draining too much from the battery



	base.calibrate(); // Calibrates the sensors for the base

	// Reset the base motor positions to 0 for accuracy
	leftMotors.tare_position_all();
	rightMotors.tare_position_all();

	// Set the base motors to coast (i.e. when a movement is over gradually slow down) to avoid burnout, should only add some minor drift
	leftMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);
	rightMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);

	verticalRotationSensor.reset_position();
	horizontalRotationSensor.reset_position();



	// Default states for autonomous, intake, and arm
	global::autonSelected = states::autonStates::RedPositiveCorner;

	global::intakeState = states::intakeStates::Resting;
	global::armState = states::armStates::Resting;
	


	// Same logic as above
	intake.tare_position();

	// Set the intake motors to coast (i.e. when a movement is over gradually slow down) to avoid burnout, should only add some minor drift
	intake.set_brake_mode(pros::v5::MotorBrake::coast);

	arm.tare_position_all();

	// Set the arm motors to brake (i.e. when a movement is over gradually slow down) to hold position when 
	arm.set_brake_mode_all(pros::v5::MotorBrake::brake);

	std::cout << intake.get_position_all()[0] << intake.get_position_all()[1] << std::endl;


	pros::task_t updateIntakeSpeed = pros::Task::create(handleIntake, "Update intake speed"); // Create a task for moving the intake so that the rest of the code will not 
																											  // potentially be interrupted

	pros::task_t updateArmPosition = pros::Task::create(handleArm, "Update intake speed"); // Same logic as the intake task


	

	// Useful for checking when our base is overheating so that we can let it rest
	if (leftMotors.is_over_temp_all()[0] || leftMotors.is_over_temp_all()[1] || leftMotors.is_over_temp_all()[2])
		controller.rumble("- _ _");

	if (rightMotors.is_over_temp_all()[0] || rightMotors.is_over_temp_all()[1] || rightMotors.is_over_temp_all()[2])
		controller.rumble("_ - -");

	if (intake.is_over_temp())
		controller.rumble("_ - _");
}



void disabled() {}



void competition_initialize() {
	autonSelectorInitialization(); // Creates buttons on the brain screen for selecting autonomous routines
}