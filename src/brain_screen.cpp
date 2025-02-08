#include "pros/rtos.hpp"
#include "pros/llemu.hpp"

#include "my_includes/ports.h"



LV_IMG_DECLARE(brain_image);

lv_obj_t *brainImg = lv_obj_create(NULL);

lv_obj_t *img = lv_img_create(brainImg);

void setBrainImage() {
    lv_img_set_src(img, &brain_image);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    lv_scr_load(brainImg);
}

void robotCoordinates() {
    pros::lcd::initialize(); // Initialize brain screen

    pros::Task screen_task([&]() {
        while (true) {
            // Print robot location to the brain screen
            pros::lcd::print(0, "X: %f", base.getPose().x);
            pros::lcd::print(1, "Y: %f", base.getPose().y);
            pros::lcd::print(2, "Theta: %f", base.getPose().theta);
            
            
            
            pros::delay(5); // Delay to save resources
        }
    });
}