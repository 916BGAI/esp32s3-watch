#include "../ui.h"
#include "lc76g.h"
#include "ui_menu_screen.h"
#include "esp_log.h"
#include "ui_gps_screen.h"

static gps_screen_t *gps_screen;
static gps_t gps;

static void timer_cb(lv_timer_t *timer);
static void ui_event_gps_screen(lv_event_t *e);

void ui_gps_screen_init(void)
{
    lc76g_init();

    gps_screen = malloc(sizeof(gps_screen_t));
    gps_screen->screen = lv_obj_create(NULL);

    lv_obj_t * label_text = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(label_text, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_text, "定位");
    lv_obj_align(label_text, LV_ALIGN_DEFAULT, 20, 20);

    gps_screen->label.data = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.data, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.data,"1970/01/01");
    lv_obj_align(gps_screen->label.data, LV_ALIGN_TOP_MID, -50, 76);

    gps_screen->label.time = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.time, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.time,"00:00:00");
    lv_obj_align(gps_screen->label.time, LV_ALIGN_TOP_MID, 50, 76);

    gps_screen->label.longitude = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.longitude, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.longitude, "0.000°E");
    lv_obj_align(gps_screen->label.longitude, LV_ALIGN_TOP_MID, -55, 136);

    lv_obj_t *longitude_text = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(longitude_text, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(longitude_text, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_label_set_text(longitude_text, "经度");
    lv_obj_align_to(longitude_text, gps_screen->label.longitude, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    gps_screen->label.latitude = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.latitude, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.latitude, "0.000°N");
    lv_obj_align(gps_screen->label.latitude, LV_ALIGN_TOP_MID, 55, 136);

    lv_obj_t *latitude_text = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(latitude_text, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(latitude_text, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_label_set_text(latitude_text, "纬度");
    lv_obj_align_to(latitude_text, gps_screen->label.latitude, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    gps_screen->label.altitude = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.altitude, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.altitude, "0.00m");
    lv_obj_align(gps_screen->label.altitude, LV_ALIGN_TOP_MID, -55, 196);

    lv_obj_t *altitude_text = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(altitude_text, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(altitude_text, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_label_set_text(altitude_text, "海拔");
    lv_obj_align_to(altitude_text, gps_screen->label.altitude, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    gps_screen->label.speed = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(gps_screen->label.speed, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gps_screen->label.speed, "0.000m/s");
    lv_obj_align(gps_screen->label.speed, LV_ALIGN_TOP_MID, 55, 196);

    lv_obj_t *speed_text = lv_label_create(gps_screen->screen);
    lv_obj_set_style_text_font(speed_text, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(speed_text, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_label_set_text(speed_text, "速度");
    lv_obj_align_to(speed_text, gps_screen->label.speed, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    gps_screen->timer = lv_timer_create(timer_cb, 1000, NULL);
    lv_timer_ready(gps_screen->timer);

    lv_obj_add_event_cb(gps_screen->screen, ui_event_gps_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(gps_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

static void timer_cb(lv_timer_t *timer)
{
    char longitude[12], latitude[12], altitude[8], speed[12];
    gps = lc76g_get_data();
    sprintf(longitude, "%.03f°E", gps.longitude);
    sprintf(latitude, "%.03f°N", gps.latitude);
    sprintf(altitude, "%.02fm", gps.altitude);
    sprintf(speed, "%.03fm/s", gps.speed);

    lv_label_set_text_fmt(gps_screen->label.data,"%04d/%02d/%02d", gps.date.year+2000, gps.date.month, gps.date.day);
    lv_label_set_text_fmt(gps_screen->label.time,"%02d:%02d:%02d", gps.tim.hour+8, gps.tim.minute, gps.tim.second);
    lv_label_set_text(gps_screen->label.longitude, longitude);
    lv_label_set_text(gps_screen->label.latitude, latitude);
    lv_label_set_text(gps_screen->label.altitude, altitude);
    lv_label_set_text(gps_screen->label.speed, speed);
}

extern menu_screen_t *menu_screen;
static void ui_event_gps_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
        lv_obj_set_tile_id(menu_screen->tv, 4, 0, LV_ANIM_OFF);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        lv_timer_del(gps_screen->timer);
        free(gps_screen);
        gps_screen = NULL;
        lc76g_deinit();
    }
}