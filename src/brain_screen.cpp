#include "pros/rtos.hpp"
#include "pros/llemu.hpp"


#include "ports.h"



LV_IMG_DECLARE(brain_image);
lv_obj_t *img = lv_img_create(lv_scr_act());

void setBrainImage() {
    lv_img_set_src(img, &brain_image);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

void robotCoordinates() {
    pros::lcd::initialize(); // initialize brain screen

    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", base.getPose().x);
            pros::lcd::print(1, "Y: %f", base.getPose().y);
            pros::lcd::print(2, "Theta: %f", base.getPose().theta);
            // delay to save resources
            pros::delay(20);
        }
    });
}