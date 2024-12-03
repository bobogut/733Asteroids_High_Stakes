#include "liblvgl/core/lv_disp.h"
#include "liblvgl/font/lv_symbol_def.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/misc/lv_style.h"
#include "liblvgl/widgets/lv_btn.h"
#include "liblvgl/widgets/lv_label.h"


#include "global_var.h"
#include "states.h"



// Autonomous selector screens (think of them like website pages)
lv_obj_t *homeScreen = lv_obj_create(NULL);
lv_obj_t *redScreen = lv_obj_create(NULL);
lv_obj_t *blueScreen = lv_obj_create(NULL);



// Button declerations
lv_obj_t *skillsBtn = lv_btn_create(homeScreen); // Indicates that this button (for autonomous skills) was specifically created for the home page (thus only visible when the home 
                                                        // page is loaded).

// Buttons specifically for red side autonomous
lv_obj_t *redSide = lv_btn_create(homeScreen);
lv_obj_t *redPositiveCornerBtn = lv_btn_create(redScreen);
lv_obj_t *redNegativeCornerBtn = lv_btn_create(redScreen);
lv_obj_t *redSoloAWPBtn = lv_btn_create(redScreen);

// Buttons specifically for blue side autonomous
lv_obj_t *blueSide = lv_btn_create(homeScreen); 
lv_obj_t *bluePositiveCornerBtn = lv_btn_create(blueScreen);
lv_obj_t *blueNegativeCornerBtn = lv_btn_create(blueScreen);
lv_obj_t *blueSoloAWPBtn = lv_btn_create(blueScreen);

// Buttons to navigage the screens
lv_obj_t *backRed = lv_btn_create(redScreen);
lv_obj_t *backBlue = lv_btn_create(blueScreen);



// Button style declerations (how they look)
lv_style_t styleBtn;

lv_style_t styleRed;
lv_style_t styleBlue;
lv_style_t styleGrey;

lv_style_t styleRedSelected;
lv_style_t styleBlueSelected;
lv_style_t styleGreySelected;

void basicStyleInitialization(lv_style_t * style, lv_palette_t color) {
    lv_style_init(style);                                          // Initializes the style
    lv_style_set_bg_color(style, lv_palette_main(color)); // Simply setting the background color to associated color
}

void seclectedStyleInitialization(lv_style_t * style, lv_palette_t color) {
    lv_style_init(style);
    lv_style_set_border_width(style, 10);                             // Sets the border width to 10 pixels
    lv_style_set_border_opa(style, 255);                              // Sets the border opacity (i.e. transparency) to zero
    lv_style_set_border_color(style, lv_color_white());               // Sets the border color to white
    lv_style_set_bg_color(style, lv_palette_darken(color, 3)); // Sets the background color to a darker version
}

// Assigning aesthetics to the style
void styleInitialization() {
    lv_style_set_radius(&styleBtn, 10);                        // Rounds the corners of the buttons
    lv_style_set_bg_opa(&styleBtn, LV_OPA_COVER);              // Applies some opacity to the background
    lv_style_set_text_color(&styleBtn, lv_color_black());      // Makes the text on the colors black
    lv_style_set_text_font(&styleBtn, &lv_font_montserrat_24); // Increases font size


    // Style for red, blue, and grey buttons
    basicStyleInitialization(&styleRed, LV_PALETTE_RED);
    basicStyleInitialization(&styleBlue, LV_PALETTE_BLUE);
    basicStyleInitialization(&styleGrey, LV_PALETTE_GREY);



    // Style for selected buttons
    seclectedStyleInitialization(&styleRedSelected, LV_PALETTE_RED);
    seclectedStyleInitialization(&styleBlueSelected, LV_PALETTE_BLUE);
    seclectedStyleInitialization(&styleGreySelected, LV_PALETTE_GREY);
}

// How buttons are visually updated to reflect the chosen autonomous
void updateStates() {
    // Removes the checked state to return them to their original color
    lv_obj_clear_state(skillsBtn, LV_STATE_CHECKED);

    lv_obj_clear_state(redSide, LV_STATE_CHECKED);
    lv_obj_clear_state(redPositiveCornerBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(redNegativeCornerBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(redSoloAWPBtn, LV_STATE_CHECKED);

    lv_obj_clear_state(blueSide, LV_STATE_CHECKED);
    lv_obj_clear_state(bluePositiveCornerBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(blueNegativeCornerBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(blueSoloAWPBtn, LV_STATE_CHECKED);


    // Adds the checked state to the button matching the autonomous selected
    if (autonSelected == SKILLS)
        lv_obj_add_state(skillsBtn, LV_STATE_CHECKED);
    else if (autonSelected == REDPOSITIVECORNER)
        lv_obj_add_state(redPositiveCornerBtn, LV_STATE_CHECKED);
    else if (autonSelected == REDNEGATIVECORNER)
        lv_obj_add_state(redNegativeCornerBtn, LV_STATE_CHECKED);
    else if (autonSelected == REDSOLOAWP)
        lv_obj_add_state(redSoloAWPBtn, LV_STATE_CHECKED);
    else if (autonSelected == BLUEPOSITIVECORNER)
        lv_obj_add_state(bluePositiveCornerBtn, LV_STATE_CHECKED);
    else if (autonSelected == BLUENEGATIVECORNER)
        lv_obj_add_state(blueNegativeCornerBtn, LV_STATE_CHECKED);
    else if (autonSelected == BLUESOLOAWP)
        lv_obj_add_state(blueSoloAWPBtn, LV_STATE_CHECKED);

    if (autonSelected == REDPOSITIVECORNER || autonSelected == REDNEGATIVECORNER || autonSelected == REDSOLOAWP)
        lv_obj_add_state(redSide, LV_STATE_CHECKED);
    else if (autonSelected == BLUEPOSITIVECORNER || autonSelected == BLUENEGATIVECORNER || autonSelected == BLUESOLOAWP)
        lv_obj_add_state(blueSide, LV_STATE_CHECKED);
}

// What happens when each button is pressed 
void buttonPress(lv_event_t *btnPressed) {
    lv_obj_t *btn = lv_event_get_target(btnPressed);

    // Home screen buttons
    if (btn == skillsBtn) {
        if (autonSelected != SKILLS) // Serves as a toggle, in this case for selecting skills
            autonSelected = SKILLS;
        else
            autonSelected = NONE;
    }
    else if (btn == redSide)
        lv_scr_load(redScreen); // Loads the associated screen, in this case for red autonomous
    else if (btn == blueSide)
        lv_scr_load(blueScreen);



    // Red side buttons
    if (btn == redPositiveCornerBtn) {
        if (autonSelected != REDPOSITIVECORNER)
            autonSelected = REDPOSITIVECORNER;
        else
            autonSelected = NONE;
    }
    else if (btn == redNegativeCornerBtn) {
        if (autonSelected != REDNEGATIVECORNER)
            autonSelected = REDNEGATIVECORNER;
        else
            autonSelected = NONE;
    }
    else if (btn == redSoloAWPBtn) {
        if (autonSelected != REDSOLOAWP)
            autonSelected = REDSOLOAWP;
        else
            autonSelected = NONE;
    }



    // Blue side buttons
    if (btn == bluePositiveCornerBtn) {
        if (autonSelected != BLUEPOSITIVECORNER)
            autonSelected = BLUEPOSITIVECORNER;
        else
            autonSelected = NONE;
    }
    else if (btn == blueNegativeCornerBtn) {
        if (autonSelected != BLUENEGATIVECORNER)
            autonSelected = BLUENEGATIVECORNER;
        else
            autonSelected = NONE;
    }
    else if (btn == blueSoloAWPBtn) {
        if (autonSelected != BLUESOLOAWP)
            autonSelected = BLUESOLOAWP;
        else
            autonSelected = NONE;
    }



    if (btn == backRed || btn == backBlue) // Back buttons to return to the homescreen
        lv_scr_load(homeScreen);



    updateStates(); // Visualy updates buttons to reflect the current autonomous selection
}

// Provides the button with characteristics
void buttonInitialization(lv_obj_t *btn, int x, int y, int width, int height, const char *name) {
    // Determining the position, dimension, and fuction of the button
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, width, height); // Note to self screen is 480 x 240
    lv_obj_add_event_cb(btn, buttonPress, LV_EVENT_CLICKED, NULL);

    // Determing the appearance of the buttons
    lv_obj_add_style(btn, &styleBtn, 0);

    // For labelling/naming the button
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, name);
    lv_obj_center(label);
}

// Assigns styles to buttons which ends up coloring them
void styleButtons() {
    // Styling the skills button
    lv_obj_add_style(skillsBtn, &styleGrey, LV_STATE_DEFAULT);
    lv_obj_add_style(skillsBtn, &styleGreySelected, LV_STATE_CHECKED);



    // Styling the red side autonomous buttons
    lv_obj_add_style(redSide, &styleRed, LV_STATE_DEFAULT);
    lv_obj_add_style(redSide, &styleRedSelected, LV_STATE_CHECKED);

    lv_obj_add_style(redPositiveCornerBtn, &styleRed, LV_STATE_DEFAULT);
    lv_obj_add_style(redPositiveCornerBtn, &styleRedSelected, LV_STATE_CHECKED);

    lv_obj_add_style(redNegativeCornerBtn, &styleRed, LV_STATE_DEFAULT);
    lv_obj_add_style(redNegativeCornerBtn, &styleRedSelected, LV_STATE_CHECKED);

    lv_obj_add_style(redSoloAWPBtn, &styleRed, LV_STATE_DEFAULT);
    lv_obj_add_style(redSoloAWPBtn, &styleRedSelected, LV_STATE_CHECKED);




    // Styling the blue side autonomous buttons
    lv_obj_add_style(blueSide, &styleBlue, LV_STATE_DEFAULT);
    lv_obj_add_style(blueSide, &styleBlueSelected, LV_STATE_CHECKED);

    lv_obj_add_style(bluePositiveCornerBtn, &styleBlue, LV_STATE_DEFAULT);
    lv_obj_add_style(bluePositiveCornerBtn, &styleBlueSelected, LV_STATE_CHECKED);
    

    lv_obj_add_style(blueNegativeCornerBtn, &styleBlue, LV_STATE_DEFAULT);
    lv_obj_add_style(blueNegativeCornerBtn, &styleBlueSelected, LV_STATE_CHECKED);

    lv_obj_add_style(blueSoloAWPBtn, &styleBlue, LV_STATE_DEFAULT);
    lv_obj_add_style(blueSoloAWPBtn, &styleBlueSelected, LV_STATE_CHECKED);



    // Styling the back buttons
    lv_obj_add_style(backRed, &styleGrey, LV_STATE_DEFAULT);
    lv_obj_add_style(backBlue, &styleGrey, LV_STATE_DEFAULT);
}

// Creates the buttons on the screen
void autonSelectorInitialization() {
    // lv_disp_set_rotation(NULL, LV_DISP_ROT_270); // Look at for rotating https://github.com/lvgl/lvgl/issues/4060

    lv_scr_load(homeScreen); // Default screen



    // Intiation and application of styles
    styleInitialization();
    styleButtons();



    // Creation of buttons
    buttonInitialization(skillsBtn, 12, 12, 144, 192, "Skills");

    buttonInitialization(redSide, 168, 12, 144, 192, "Red Alliance");
    buttonInitialization(redPositiveCornerBtn, 12, 12, 144, 152, LV_SYMBOL_PLUS);
    buttonInitialization(redNegativeCornerBtn, 168, 12, 144, 152, LV_SYMBOL_MINUS);
    buttonInitialization(redSoloAWPBtn, 324, 12, 144, 152, "SOLO AWP");

    buttonInitialization(blueSide, 324, 12, 144, 192, "Blue Alliance");
    buttonInitialization(bluePositiveCornerBtn, 12, 12, 144, 152, LV_SYMBOL_PLUS);
    buttonInitialization(blueNegativeCornerBtn, 168, 12, 144, 152, LV_SYMBOL_MINUS);
    buttonInitialization(blueSoloAWPBtn, 324, 12, 144, 152, "SOLO AWP");

    buttonInitialization(backRed, 12, 176, 456, 52, "Back");
    buttonInitialization(backBlue, 12, 176, 456, 52, "Back");
}