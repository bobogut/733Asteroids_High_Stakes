// Relevant PROS library are used for tasks and checking motor temperature (embeded in main.h). Other relevant header files are included to declare the fuynction for out intake task and
// declare relevant motors to modify their attributes.
#include "main.h"

#include "my_includes/auton_selector.h"
#include "my_includes/global_var.h"
#include "my_includes/ports.h"
#include "my_includes/robot_functions.h"
#include "my_includes/states.h"
#include "pros/motors.h"



void initialize() {
	optical.set_led_pwm(0); // Turn off the LED on the optical sensor to avoid draining too much from the battery

	optical.set_integration_time(20);



	base.calibrate(); // Calibrates the sensors for the base

	// Reset the base motor positions to 0 for accuracy
	leftMotors.tare_position_all();
	rightMotors.tare_position_all();

	// Set the base motors to coast (i.e. when a movement is over gradually slow down) to avoid burnout, should only add some minor drift
	base.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	verticalRotationSensor.reset_position();
	horizontalRotationSensor.reset_position();



	// Default states for autonomous, intake, and arm
	global::autonSelected = states::autonStates::None;

	global::intakeState = states::intakeStates::Resting;

	global::armState = states::armStates::Resting;
	


	// Same logic as above
	intakeFirstStage.tare_position();
	intakeSecondStage.tare_position();

	// Set the intake motors to coast (i.e. when a movement is over gradually slow down) to avoid burnout, should only add some minor drift
	intakeFirstStage.set_brake_mode(pros::v5::MotorBrake::coast);
	intakeSecondStage.set_brake_mode(pros::v5::MotorBrake::coast);

	arm.tare_position_all();

	// Set the arm motors to brake (i.e. when a movement is over gradually slow down) to hold position when 
	arm.set_brake_mode(pros::v5::MotorBrake::hold);



	pros::task_t updateIntakeSpeed = pros::Task::create(handleIntake, "Update intake speed"); // Create a task for moving the intake so that the rest of the code will not 
																											  // potentially be interrupted

	pros::task_t updateArmPosition = pros::Task::create(handleArm, "Update intake speed"); // Same logic as the intake task
}



void disabled() {}



void competition_initialize() {
	autonSelectorInitialization(); // Creates buttons on the brain screen for selecting autonomous routines
}