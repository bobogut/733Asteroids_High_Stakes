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

        float reflectY(float originalY) {
            return -originalY;
        }

        float reflectAngleSkills(float originalAngle) {
            return 540 - originalAngle;
        }

        lemlib::DriveSide reflectDriveSide(lemlib::DriveSide lockedSide) {
            if (lockedSide == lemlib::DriveSide::LEFT)
                return lemlib::DriveSide::RIGHT;
            else
                return lemlib::DriveSide::LEFT;
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

        void reflectSkills() {
            y = CoordianteInfo::reflectY(y);
        }

        void newPoint(float x, float y) {
            this->x = x;
            this->y = y;
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

        void reflectSkills() {
            y = CoordianteInfo::reflectY(y);
            theta = CoordianteInfo::reflectAngleSkills(theta);
        }

        void newPose(float x, float y, float theta) {
            this->x = x;
            this->y = y;
            this->theta = theta;
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

        void reflectSkills() {
            y = CoordianteInfo::reflectY(y);
            lockedSide = CoordianteInfo::reflectDriveSide(lockedSide);
        }

        void newSwingPoint(float x, float y, lemlib::DriveSide lockedSide) {
            this->x = x;
            this->y = y;
            this->lockedSide = lockedSide;
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

        void reflectSkills() {
            theta = CoordianteInfo::reflectAngleSkills(theta);
            lockedSide = CoordianteInfo::reflectDriveSide(lockedSide);
        }

        void newPoint(float theta, lemlib::DriveSide lockedSide) {
            this->theta = theta;
            this->lockedSide = lockedSide;
        }
};