#pragma once

#include <cstdlib>
#include <math.h>

class MyPID {
    protected:
        const float kP;
        const float kI; // We are probably running a basic PD controller so kI and anti-windup will not be used too much 
        const float kD;

        const float antiWindup;

        const float exitRange; // When the error is small enough that we are satisfied with not moving anymore
        const int exitTimeout; // How long we should wait in our exit range before stopping the arm

        const int minSpeed;

    private:
        float iError = 0;      // Integral error or error built up over time
        float lastError = NAN; // Used for calculating derivative error or rate of error change
        float dError = 0;      // Derivative error or rate of error change

        float velocity = 0;    // Stores the computed velocity for the PID

        int trackingTime = 0;  // Used for tracking time until early exit

    public:
        MyPID(float kP, float kI, float kD, float antiWindup, float exitRange, int exitTimeout, int minSpeed) : kP(kP), kI(kI), kD(kD), antiWindup(antiWindup), exitRange(exitRange), 
                                                                                                                exitTimeout(exitTimeout), minSpeed(minSpeed) {};

        // Updates all the necessary PID variables
        void update(float error) {
            if (lastError != lastError)
                lastError = error;



            if (abs(error) < exitRange)
                trackingTime += 5;
            else
                trackingTime = 0;



            iError += error;

            if (iError > antiWindup)
                iError = 0;

            dError = error - lastError;

            lastError = error;



            velocity = kP * error + kI * iError + kD * dError / 5;

            velocity < minSpeed ? velocity = minSpeed : velocity;
        }

        float getVelocity() {
            return velocity; // Returns the velocity
        }

        bool earlyExit() {
            return trackingTime > exitTimeout; // Returns whether or not the time range for early exit has been met
        }

        // Resets all the relevant variables for the next PID motion
        void reset() {
            lastError = NAN;
            iError = 0;
            trackingTime = 0;
        }
};