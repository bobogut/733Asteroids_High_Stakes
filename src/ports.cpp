// Lemlib chassis library used for chassis configuration. Relevant PROS library for motor groups, inertial sensors, optical sensors, and pneumatics.
#include "lemlib/chassis/chassis.hpp"

#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/optical.hpp"



// Set up for the drivetrain motors (1, 2, 3, 4, 5, and 6) on the left and right sides respectively
pros::MotorGroup leftMotors({-19, -18, -17}, pros::v5::MotorGears::blue,
                            pros::v5::MotorUnits::degrees);
pros::MotorGroup rightMotors({14, 15, 16}, pros::v5::MotorGears::blue,
                             pros::v5::MotorUnits::degrees);

// Lemlib set up for the full drivetrain
lemlib::Drivetrain drivetrain
{
    &leftMotors,                   // The motors on the left side of the drivetrain
    &rightMotors,                 // The motors on the right side of the drivetrain
    10.305,                        // Track width, i.e. the distance between wheels on opposite sides of the drivetrain
    lemlib::Omniwheel::NEW_325, // We are using the VEX 3.25" anti-static wheels
    450,                                  // We are running a 450 RPM base
    2                         // The horizontal drift
};

pros::IMU imu(10); // Set up for the inertial sensor (imu) on port 7

// Set up for the tracking wheels using rotational sensors on port 8 and 9
pros::Rotation verticalRotationSensor(-8);
lemlib::TrackingWheel verticalTracking(&verticalRotationSensor, 2.0, 0.5559155); // The second parameter indicates that
                                                                                                                                      // we are using the VEX 2" anti-static
                                                                                                                                      // wheels. The third parameter indicates
                                                                                                                                      // the offset from the center (in this case
                                                                                                                                      // the vertical offset is accounted for)

pros::Rotation horizontalRotationSensor(4);
lemlib::TrackingWheel horizontalTracking(&horizontalRotationSensor,
                                         2.0, 2.3059155);

lemlib::OdomSensors sensors // Putting together everything to fully set up odometry in Lemlib
(
    &verticalTracking,   // For tracking movement on the y-axis
    nullptr,   // No second vertical tracking wheel
    &horizontalTracking, // For tracking movement on the x-axis
    nullptr, // No second horizontal tracking wheel
    &imu             // For determining heading (rotation)

);

lemlib::ControllerSettings lateralController // PID constants for moving foward/backward
(
    7.5, // 7.5                  // Proportional gains, i.e. the weight of error on the calculation
    0.1,                    // Integral gains, i.e. the weight of error accumulation on the calculation
    25, // 25                    // Derivative gains, i.e. the weight of error change on the calculation
    1.00335, // Anti windup
    1,            // Small error timeout is considered when the robot is within 1" of its goal
    100, // When error is in the small range for long enough move on to the next motion
    3,            // Large error timeout is considered when the robot is within 3" of its goal
    500, // When error is in the large range for long enough move on to the next motion
    0                   // Maximum acceleration
);

lemlib::ControllerSettings angularController // PID constants for turning
(
    4,
    0.45,
    30, // 18.75
    -1.15725333333333,
    1,100,3,500,0
);

lemlib::Chassis base // Lemlib set up for the full base
(
    drivetrain,
    lateralController,
    angularController,
    sensors
);

pros::Motor intakePre(12, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::rotations); // Intake motor
pros::Motor intakeHook(11, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::rotations);

pros::Optical optical(6); // Optical sensor for ring sorter

pros::adi::Pneumatics mogoClamp('a', false); // Technically starts extended due to wiring
pros::adi::Pneumatics doinker('b', false);