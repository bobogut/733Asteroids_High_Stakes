#pragma once

#include <cstdlib>
#include <math.h>

class MyPID {
    protected:
        const float kP;
        const float kI; // We are probably running a basic PD controller so kI and anti-windup will not be used too much 
        const float kD;

        const float antiWindup;

        const float exitRange;      // When the error is small enough that we are satisfied with not moving anymore
        const int exitTimeout;      // How long we should wait in our exit range before stopping the arm
        const int largeExitTimeout; // If we are unable to enter the exit range or settle by itself within a reasonable time, just stop the motion

        const int minSpeed;

    private:
        float iError = 0;          // Integral error or error built up over time
        float lastError = NAN;     // Used for calculating derivative error or rate of error change
        float dError = 0;          // Derivative error or rate of error change

        float velocity = 0;        // Stores the computed velocity for the PID

        int trackingTime = 0;      // Used for tracking time until early exit
        int trackingTimeLarge = 0; // Used for tracking the time the overall motion takes

    public:
        MyPID(float kP, float kI, float kD, float antiWindup, float exitRange, int exitTimeout, int largeExitTimeout, int minSpeed) : kP(kP), kI(kI), kD(kD), antiWindup(antiWindup), 
                                                                                                                                      exitRange(exitRange), exitTimeout(exitTimeout),
                                                                                                                                      largeExitTimeout(largeExitTimeout), minSpeed(minSpeed) {};

        // Updates all the necessary PID variables
        void update(float error) {
            if (lastError != lastError)
                lastError = error;



            if (abs(error) <= exitRange)
                trackingTime += 5;
            else 
                trackingTime = 0;

            trackingTimeLarge += 5;



            iError += error;

            if (iError > antiWindup)
                iError = 0;

            dError = error - lastError;

            lastError = error;

            velocity = kP * error + kI * iError + kD * dError / 5;

            abs(velocity) < minSpeed ? velocity = minSpeed : velocity; // If we have a minimum speed for the PID set it to the minimum speed
        }

        float getVelocity() {
            return velocity; // Returns the velocity calculated by the PID
        }

        float getTime() {
            return trackingTimeLarge; // Return total time PID has been running for
        }

        bool earlyExit() {
            if (exitRange > 0 && largeExitTimeout > 0)
                return trackingTime >= exitTimeout || trackingTimeLarge >= largeExitTimeout; // Returns whether or not the time range for early exit has been met
            else if (exitRange > 0 && largeExitTimeout == 0)
                return trackingTime >= exitTimeout;
            else if (exitRange == 0 && largeExitTimeout > 0)
                return trackingTimeLarge >= largeExitTimeout;
            else
                return false;
        }

        // Resets all the relevant variables for the next PID motion
        void reset() {
            lastError = NAN;
            iError = 0;
            trackingTime = 0;
            trackingTimeLarge = 0;
        }
};