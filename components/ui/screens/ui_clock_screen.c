#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../ui.h"
#include "ui_helpers.h"
#include "ui_clock_screen.h"

clock_screen_t clock_screen;

static struct timeval timeval = {
    .tv_sec = 0,
    .tv_usec = 0,
};

// static void ui_calibrate_update(lv_timer_t *timer);
static void ui_clock_update(lv_timer_t *timer);
static void ui_event_clock_screen(lv_event_t *e);

void ui_clock_screen_init(void)
{
    clock_screen.screen = lv_obj_create(NULL);
    lv_obj_clear_flag(clock_screen.screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(clock_screen.screen, &ui_font_sarasa16, LV_PART_MAIN | LV_STATE_DEFAULT);

    clock_screen.tv = lv_tileview_create(clock_screen.screen);
    lv_obj_set_size(clock_screen.tv, 240, 40);
    lv_obj_align(clock_screen.tv, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_bg_opa(clock_screen.tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(clock_screen.tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_font(clock_screen.tv, &ui_font_sarasa18, LV_PART_MAIN | LV_STATE_DEFAULT);
    clock_screen.tile.time = lv_tileview_add_tile(clock_screen.tv, 0, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    clock_screen.label.time = lv_label_create(clock_screen.tile.time);
    lv_label_set_text(clock_screen.label.time, "0 0 : 0 0 : 00");
    lv_obj_center(clock_screen.label.time);
    clock_screen.tile.date = lv_tileview_add_tile(clock_screen.tv, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    clock_screen.label.date = lv_label_create(clock_screen.tile.date);
    lv_label_set_text(clock_screen.label.date, "2024 年 01 月 01 日");
    lv_obj_center(clock_screen.label.date);

    // clock_screen.timer.calibrate = lv_timer_create(ui_time_calibrate, 60 * 60 * 1000, NULL);
    // ui_time_calibrate(clock_screen.timer.calibrate);
    clock_screen.timer.clock = lv_timer_create(ui_clock_update, 1000, NULL);
    ui_clock_update(clock_screen.timer.clock);

    clock_screen.clock_contanier = lv_obj_create(clock_screen.screen);
    lv_obj_clear_flag(clock_screen.clock_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(clock_screen.clock_contanier, 220, 220);
    lv_obj_align(clock_screen.clock_contanier, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_bg_color(clock_screen.clock_contanier, lv_color_hex(0x8C40DE),
                              LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(clock_screen.screen, ui_event_clock_screen, LV_EVENT_ALL, NULL);
}

static void ui_event_clock_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_menu_screen_init);
    }
}

// void ui_time_calibrate(lv_timer_t *timer)
// {
//
// }

void ui_clock_update(lv_timer_t *timer)
{
    static struct tm *timeinfo;

    gettimeofday(&timeval, NULL);
    timeinfo = localtime(&timeval.tv_sec);
    char time_str[40];
    sprintf(time_str, "%02d : %02d : %02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    lv_label_set_text_fmt(clock_screen.label.time, "%s", time_str);
    char data_str[48];
    sprintf(data_str, "%04d 年 %02d 月 %02d 日", timeinfo->tm_year+1900, timeinfo->tm_mon, timeinfo->tm_mday);
    lv_label_set_text_fmt(clock_screen.label.date, "%s", data_str);
}