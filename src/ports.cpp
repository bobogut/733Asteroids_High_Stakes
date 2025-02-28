// Lemlib chassis library used for chassis configuration. Relevant PROS library for motor groups, inertial sensors, optical sensors, and pneumatics.
#include <cmath>

#include "pros/abstract_motor.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"



/*
Bad ports
8, 9

4?
*/


// Set up for the drivetrain motors on the left and right sides respectively
pros::MotorGroup leftMotors({-1, -3, -13}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::MotorGroup rightMotors({5, 6,19}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

// Lemlib set up for the full drivetrain
lemlib::Drivetrain drivetrain {
    &leftMotors,                   // The motors on the left side of the drivetrain
    &rightMotors,                 // The motors on the right side of the drivetrain
    10.305,                        // Track width, i.e. the distance between wheels on opposite sides of the drivetrain
    lemlib::Omniwheel::NEW_325, // We are using the VEX 3.25" anti-static wheels
    450,                                  // We are running a 450 RPM base
    2                         // The horizontal drift
};

pros::IMU imu(11); // Set up for the inertial sensor (imu) on port 17

// Set up for the tracking wheels using rotational sensors on port 8 and 9
pros::Rotation verticalRotationSensor(-2);
lemlib::TrackingWheel verticalTracking(&verticalRotationSensor, 2.0, 0.5559155); // The second parameter indicates that
                                                                                                                 // we are using the VEX 2" anti-static
                                                                                                                 // wheels. The third parameter indicates
                                                                                                                 // the offset from the center (in this case
                                                                                                                 // the vertical offset is accounted for)

pros::Rotation horizontalRotationSensor(19);
lemlib::TrackingWheel horizontalTracking(&horizontalRotationSensor,
                                         2.0, 2.3059155);

// Putting together everything to fully set up odometry in Lemlib
lemlib::OdomSensors sensors(
    &verticalTracking,   // &verticalTracking,   // For tracking movement on the y-axis
    nullptr,   // No second vertical tracking wheel
    &horizontalTracking, // For tracking movement on the x-axis
    nullptr, // No second horizontal tracking wheel
    &imu             // For determining heading (rotation)

);

// PID constants for moving foward/backward
lemlib::ControllerSettings lateralController(
    6, // 5                  // Proportional gains, i.e. the weight of error on the calculation
    0,                    // Integral gains, i.e. the weight of error accumulation on the calculation 0.1
    10, // 25                    // Derivative gains, i.e. the weight of error change on the calculation
    0, // Anti windup 1.00335
    1,            // Small error timeout is considered when the robot is within 1" of its goal
    500, // When error is in the small range for long enough move on to the next motion
    3,            // Large error timeout is considered when the robot is within 3" of its goal
    1500, // When error is in the large range for long enough move on to the next motion
    0                   // Maximum acceleration
);

// PID constants for turning
lemlib::ControllerSettings angularController(
    4,
    0, // 0.45
    30, // 3
    0, //-1.15725333333333
    1,
    500,
    5,
    1500, 
    0 //1,100,3,500
);

// Lemlib set up for the full base
lemlib::Chassis base(drivetrain, lateralController, angularController, sensors);



pros::Motor arm(-7, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::degrees); // Right motor reversed



pros::Motor intakeFirstStage(12, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::degrees);
pros::Motor intakeSecondStage(21, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);



pros::Optical optical(4); // Optical sensor for ring sorter



pros::adi::Pneumatics mogoClamp('d', false);
pros::adi::Pneumatics doinker('h', false);
// pros::adi::Pneumatics intakePiston('c', true);