#include "ui.h"
#include "ui_helpers.h"

// SCREEN: ui_main_screen
lv_obj_t *ui_main_screen;
lv_obj_t *ui_image1;
lv_obj_t *ui_button1;
lv_obj_t *ui_label1;

// SCREEN: ui_menu_screen
lv_obj_t *ui_menu_screen;
lv_obj_t *ui____initial_actions0;

void ui_event_main_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_GESTURE &&
        lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_menu_screen_init);
    }
}

void ui_event_button1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        led_event_callback(e);
    }
}

void ui_event_menu_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_GESTURE &&
        lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_main_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp,
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_main_screen_init();
    ui_menu_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_main_screen);
}