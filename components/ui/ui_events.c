#include "ui.h"

void ui_event_main_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_menu_screen_init);
    }
}

void ui_event_button1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED) {
        led_event_callback(e);
    }
}

void ui_event_menu_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_main_screen_init);
    }
}

void led_event_callback(lv_event_t *e)
{
    // Your code here
}