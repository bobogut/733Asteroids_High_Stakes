#pragma once

#include <cstdlib>
#include <math.h>
#include <initializer_list>
#include <iostream>

class MyPID {
    protected:
        const float kP;
        const float kI;
        const float kD;

        const float antiWindup;

        const std::initializer_list<float> exitRange;

    private:
        float lastError = NAN;
        float iError = 0;
        float dError = 0;

        int trackingTime = 0;
        float velocity;

    public:
        MyPID(float kP, float kI, float kD, float antiWindup, std::initializer_list<float> exitRange) : kP(kP), kI(kI), kD(kD), antiWindup(antiWindup), exitRange(exitRange) {
            std::cout << "Exit range is " << exitRange.size() << ", " << exitRange.begin()[0] << std::endl;
        };

        void update(float error) {
            
            if (lastError != lastError)
                lastError = error;

            // std::cout << "error is " << abs(error) << " range is " << exitRange.begin()[0] << std::endl;

            if (abs(error) < exitRange.begin()[0]) {
                trackingTime += 5;
                // std::cout << "Within range time " << trackingTime << std::endl;
            } else
                trackingTime = 0;

            // std::cout << "tracking time is " << trackingTime << std::endl;

            iError += error;

            if (iError > antiWindup)
                iError = 0;

            dError = error - lastError;

            velocity = kP * error + kI * iError + kD * dError / 5;

            lastError = error;
        }

        float getVelocity() {
            return velocity;
        }

        void reset() {
            lastError = NAN;
            iError = 0;
            trackingTime = 0;
        }

        bool earlyExit() {
            return trackingTime > exitRange.begin()[1];
        }
};