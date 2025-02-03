#pragma once

#include "lemlib/chassis/chassis.hpp"


class CoordianteInfo {
    public:
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
    public:
        float x;
        float y;

        MyPoint(float x, float y) : x(x), y(y) {}

        void reflect() {
            x = CoordianteInfo::reflectX(x);
        }
};

class MyPose : private CoordianteInfo {
    public:
        float x;
        float y;
        float theta;

        MyPose(float x, float y, float theta) : x(x), y(y), theta(theta) {}
    
        void reflect() {
            x = CoordianteInfo::reflectX(x);
            theta = CoordianteInfo::reflectAngle(theta);
        }
};

class MySwingPoint : private CoordianteInfo {
    public:
        float x;
        float y;
        lemlib::DriveSide lockedSide;

        MySwingPoint(float x, float y, lemlib::DriveSide lockedSide) : x(x), y(y), lockedSide(lockedSide) {}

        void reflect() {
            x = CoordianteInfo::reflectX(x);
            lockedSide = CoordianteInfo::reflectDriveSide(lockedSide);
        }
};

class MySwingTheta : private CoordianteInfo {
    public:
        float theta;
        lemlib::DriveSide lockedSide;

        MySwingTheta(float theta, lemlib::DriveSide lockedSide) : theta(theta), lockedSide(lockedSide) {}

        void reflect() {
            theta = CoordianteInfo::reflectAngle(theta);
            lockedSide = CoordianteInfo::reflectDriveSide(lockedSide);
        }
};