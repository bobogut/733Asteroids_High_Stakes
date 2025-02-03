#pragma once



// Lemlib chassis library used for chassis configuration. Relevant PROS library for motor groups, inertial sensors, optical sensors, and pneumatics.
#include "lemlib/chassis/chassis.hpp"

#include "pros/optical.hpp"
#include "pros/rotation.hpp"



extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::Rotation verticalRotationSensor;
extern pros::Rotation horizontalRotationSensor;

extern lemlib::Chassis base;



extern pros::MotorGroup arm;



extern pros::Motor intake;



extern pros::Optical optical;



extern pros::adi::Pneumatics mogoClamp;
extern pros::adi::Pneumatics doinker;
extern pros::adi::Pneumatics intakePiston;