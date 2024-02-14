#include "../ui.h"
#include "ui_helpers.h"

lv_obj_t *ui_clock_screen;

static void ui_event_clock_screen(lv_event_t *e);

void ui_clock_screen_init(void)
{
    ui_clock_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_clock_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(ui_clock_screen, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_clock_screen, ui_event_clock_screen, LV_EVENT_ALL, NULL);
}

static void ui_event_clock_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_menu_screen_init);
    }
}