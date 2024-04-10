#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_weather_screen.h"

static weather_screen_t *weather_screen;

static void ui_event_weather_screen(lv_event_t *e);

void ui_weather_screen_init(void)
{
    weather_screen = malloc(sizeof(weather_screen_t));

    weather_screen->screen = lv_obj_create(NULL);

    lv_obj_add_event_cb(weather_screen->screen, ui_event_weather_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(weather_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

extern menu_screen_t *menu_screen;
void ui_event_weather_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        free(weather_screen);
    }
}