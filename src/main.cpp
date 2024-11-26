#include "main.h"

#include "auton_selector.h"
#include "ports.h"
#include "robot_functions.h"



void initialize() {
	autonSelected = none;
	autonRan = false;



	intakeState = intakeResting;
	overrideIntakeState = false;
	

	intakePre.tare_position();
	intakeHook.tare_position();

	intakePre.set_brake_mode(pros::v5::MotorBrake::coast);
	intakeHook.set_brake_mode(pros::v5::MotorBrake::coast);

	pros::task_t updateIntakeSpeed = pros::Task::create(handleIntake, "Update intake speed"); // Create a task for moving the intake so that
																							  // the rest of the code will not potentially be
																							  // interrupted



	optical.set_led_pwm(0);



	leftMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);
	rightMotors.set_brake_mode_all(pros::v5::MotorBrake::coast);

	leftMotors.tare_position_all();
	rightMotors.tare_position_all();

	verticalRotationSensor.reset_position();
	horizontalRotationSensor.reset_position();

	std::cout << "Vertical is " << verticalRotationSensor.get_position() << std::endl;
	std::cout << "Horizontal is " << horizontalRotationSensor.get_position() << std::endl;


	base.calibrate(); // Calibrates the sensors for the chassis

	std::cout << "Initialization done" << std::endl;
}



void disabled() {}



void competition_initialize() {
	autonSelectorInitialization();
}