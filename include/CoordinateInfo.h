#pragma once

#include "lemlib/chassis/chassis.hpp"


class CoordianteInfo {
    float reflectAngle(float originalAngle) {
        return 360 - originalAngle;
    }

    float reflectX(float originalX) {
        return -originalX;
    }

    lemlib::DriveSide reflectDriveSide(lemlib::DriveSide lockedSide) {
        if (lockedSide == lemlib::DriveSide::LEFT)
            return lemlib::DriveSide::RIGHT;
        else
            return  lemlib::DriveSide::LEFT;
    }
};

class MyPoint : private CoordianteInfo {
    private:
        float reflectX(float originalX);

    public:
        float x;
        float y;

        MyPoint(float x, float y)  : x(x), y(y) {}

        void reflect() {
            x = reflectX(x);
        }
};

class MyPose : private CoordianteInfo {
    private:
        float reflectAngle(float originalAngle);
        float reflectX(float originalX);

    public:
        float x;
        float y;
        float theta;

        MyPose(float x, float y, float theta) : x(x), y(y), theta(theta) {}
    
        void reflect() {
            x = reflectX(x);
            theta = reflectAngle(theta);
        }
};

class MySwingPoint : private CoordianteInfo {
    private:
        float reflectX(float originalX);
        lemlib::DriveSide reflectDriveSide(lemlib::DriveSide lockedSide);

    public:
        float x;
        float y;
        lemlib::DriveSide lockedSide;

        MySwingPoint(float x, float y, lemlib::DriveSide lockedSide) : x(x), y(y), lockedSide(lockedSide) {}

        void reflect() {
            x = reflectX(x);
            lockedSide = reflectDriveSide(lockedSide);
        }
};

class MySwingTheta : private CoordianteInfo {
    private:
        float reflectAngle(float originalAngle);
        lemlib::DriveSide reflectDriveSide(lemlib::DriveSide lockedSide);

    public:
        float theta;
        lemlib::DriveSide lockedSide;

        MySwingTheta(float theta, lemlib::DriveSide lockedSide) : theta(theta), lockedSide(lockedSide) {}

        void reflect() {
            theta = reflectAngle(theta);
            lockedSide = reflectDriveSide(lockedSide);
        }
};