/**
 * @file ui_time_app.c
 * @brief 时间应用程序功能
 */

#include <time.h>
#include <sys/time.h>
#include <esp_lvgl_port.h>
#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_options_screen.h"
#include "ui_time_app.h"
#include "../main/include/sntp.h"
#include "wifi.h"

time_app_t *time_app;

static bool time_auto = false;
static struct tm *timeinfo;
static struct timeval timeval = {
    .tv_sec = 0,
    .tv_usec = 0,
};

static void sw_event_cb(lv_event_t *e);
static void time_event_cb(lv_event_t *e);
static void msgbox_event_cb(lv_event_t *e);
static void ui_event_return(lv_event_t *e);

/**
 * @brief 时间事件回调函数
 * @param e 事件指针
 */
void time_event_callback(lv_event_t *e)
{
    time_app = malloc(sizeof(time_app_t));
    options_screen_t *options_screen = lv_event_get_user_data(e);

    options_screen->app = TIME;

    lv_obj_add_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
    time_app->contanier = lv_obj_create(options_screen->screen);
    lv_obj_set_style_border_width(time_app->contanier, 0, 0);
    lv_obj_set_size(time_app->contanier, 240, 240);
    lv_obj_align(time_app->contanier, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_load_anim(time_app->contanier, options_screen->list, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 100);
    lv_label_set_text(options_screen->label.top, "日期和时间");
    lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
    lv_obj_add_event_cb(options_screen->screen, ui_event_return, LV_EVENT_ALL, options_screen);

    /* Switch of time */
    time_app->sw.contanier = lv_obj_create(time_app->contanier);
    lv_obj_set_size(time_app->sw.contanier, 210, 53);
    lv_obj_align(time_app->sw.contanier, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(time_app->sw.contanier, 0, 0);
    lv_obj_clear_flag(time_app->sw.contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(time_app->sw.contanier, lv_color_hex(0xD5DAF9), 0);
    time_app->sw.label = lv_label_create(time_app->sw.contanier);
    lv_obj_set_style_text_font(time_app->sw.label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(time_app->sw.label, LV_ALIGN_LEFT_MID, 3, 0);
    lv_label_set_text(time_app->sw.label, "自动设置时间");

    time_app->sw.btn = lv_switch_create(time_app->sw.contanier);
    lv_obj_set_style_bg_color(time_app->sw.btn, lv_color_hex(0xB1B2FF), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_size(time_app->sw.btn, 46, 29);
    lv_obj_align(time_app->sw.btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(time_app->sw.btn, sw_event_cb, LV_EVENT_ALL, options_screen);
    if (wifi_get_connected_status(0) != ESP_OK) {
        lv_obj_add_state(time_app->sw.btn, LV_STATE_DISABLED);
        lv_obj_clear_state(time_app->sw.btn, LV_STATE_CHECKED);
        time_auto = false;
    }

    time_app->date.contanier = lv_obj_create(time_app->contanier);
    lv_obj_set_size(time_app->date.contanier, 210, 53);
    lv_obj_align(time_app->date.contanier, LV_ALIGN_TOP_MID, 0, 57);
    lv_obj_set_style_border_width(time_app->date.contanier, 0, 0);
    lv_obj_clear_flag(time_app->date.contanier, LV_OBJ_FLAG_SCROLLABLE);
    time_app->date.label = lv_label_create(time_app->date.contanier);
    lv_obj_set_style_text_font(time_app->date.label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(time_app->date.label, LV_ALIGN_LEFT_MID, 3, 0);
    lv_label_set_text(time_app->date.label, "日期");
    lv_obj_add_event_cb(time_app->date.contanier, time_event_cb, LV_EVENT_LONG_PRESSED, NULL);

    time_app->date.label2 = lv_label_create(time_app->date.contanier);
    lv_obj_set_style_text_font(time_app->date.label2, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(time_app->date.label2, LV_ALIGN_RIGHT_MID, 0, 0);

    time_app->time.contanier = lv_obj_create(time_app->contanier);
    lv_obj_set_size(time_app->time.contanier, 210, 53);
    lv_obj_align(time_app->time.contanier, LV_ALIGN_TOP_MID, 0, 114);
    lv_obj_set_style_border_width(time_app->time.contanier, 0, 0);
    lv_obj_clear_flag(time_app->time.contanier, LV_OBJ_FLAG_SCROLLABLE);
    time_app->time.label = lv_label_create(time_app->time.contanier);
    lv_obj_set_style_text_font(time_app->time.label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(time_app->time.label, LV_ALIGN_LEFT_MID, 3, 0);
    lv_label_set_text(time_app->time.label, "时间");
    lv_obj_add_event_cb(time_app->time.contanier, time_event_cb, LV_EVENT_LONG_PRESSED, NULL);

    time_app->time.label2 = lv_label_create(time_app->time.contanier);
    lv_obj_set_style_text_font(time_app->time.label2, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(time_app->time.label2, LV_ALIGN_RIGHT_MID, 0, 0);

    gettimeofday(&timeval, NULL);
    timeinfo = localtime(&timeval.tv_sec);
    char time_str[24];
    sprintf(time_str, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    lv_label_set_text_fmt(time_app->time.label2, "%s", time_str);
    char data_str[43];
    sprintf(data_str, "%04d年%02d月%02d日", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
    lv_label_set_text_fmt(time_app->date.label2, "%s", data_str);

    if (time_auto) {
        lv_obj_add_state(time_app->sw.btn, LV_STATE_CHECKED);
        lv_obj_clear_flag(time_app->date.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(time_app->time.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(time_app->date.contanier, lv_color_hex(0xE0E0E0), 0);
        lv_obj_set_style_bg_color(time_app->time.contanier, lv_color_hex(0xE0E0E0), 0);
    } else {
        lv_obj_clear_state(time_app->sw.btn, LV_STATE_CHECKED);
        lv_obj_add_flag(time_app->date.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(time_app->time.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(time_app->date.contanier, lv_color_hex(0xD5DAF9), 0);
        lv_obj_set_style_bg_color(time_app->time.contanier, lv_color_hex(0xD5DAF9), 0);
    }

}

/**
 * @brief 自动设置时间事件回调函数
 * @param e 事件指针
 */
static void sw_event_cb(lv_event_t *e)
{
    const lv_event_code_t code = lv_event_get_code(e);
    const lv_obj_t *target = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target, LV_STATE_CHECKED)) {
        time_auto = true;
        lv_obj_clear_flag(time_app->date.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(time_app->time.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(time_app->date.contanier, lv_color_hex(0xE0E0E0), 0);
        lv_obj_set_style_bg_color(time_app->time.contanier, lv_color_hex(0xE0E0E0), 0);

        if (time_update() != ESP_OK) {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "同步失败", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_state(time_app->sw.btn, LV_STATE_CHECKED);
            time_auto = false;
            lv_obj_add_flag(time_app->date.contanier, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(time_app->time.contanier, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(time_app->date.contanier, lv_color_hex(0xD5DAF9), 0);
            lv_obj_set_style_bg_color(time_app->time.contanier, lv_color_hex(0xD5DAF9), 0);
        } else {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "同步成功", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        gettimeofday(&timeval, NULL);
        timeinfo = localtime(&timeval.tv_sec);
        char time_str[24], data_str[39];
        sprintf(time_str, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
        lv_label_set_text_fmt(time_app->time.label2, "%s", time_str);
        sprintf(data_str, "%04d年%02d月%02d日", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
        lv_label_set_text_fmt(time_app->date.label2, "%s", data_str);
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        time_auto = false;
        lv_obj_add_flag(time_app->date.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(time_app->time.contanier, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(time_app->date.contanier, lv_color_hex(0xD5DAF9), 0);
        lv_obj_set_style_bg_color(time_app->time.contanier, lv_color_hex(0xD5DAF9), 0);
    }
}

/**
 * @brief 时间设置事件回调函数
 * @param e 事件指针
 */
static void time_event_cb(lv_event_t *e)
{
    const lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t * mbox;

    if(target == time_app->date.contanier) {
        mbox = lv_msgbox_create(NULL, "日期", NULL, NULL, true);
    } else {
        mbox = lv_msgbox_create(NULL, "时间", NULL, NULL, true);
    }

    lv_obj_set_style_text_font(mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(mbox);
    lv_obj_set_size(mbox, 200, 200);
    lv_obj_set_style_bg_color(lv_msgbox_get_close_btn(mbox), lv_color_hex(0xB1B2FF), 0);

    if(target == time_app->date.contanier) {
        lv_obj_t *year_roller = lv_roller_create(mbox);
        const char *year_options = "2020\n2021\n2022\n2023\n2024\n2025\n2026\n2027";
        lv_roller_set_options(year_roller, year_options, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(year_roller, 3);
        lv_obj_align(year_roller, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_size(year_roller, 50, 97);
        lv_roller_set_selected(year_roller, timeinfo->tm_year-120, LV_ANIM_ON);
        lv_obj_t *month_roller = lv_roller_create(mbox);
        const char *month_options = "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12";
        lv_roller_set_options(month_roller, month_options, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(month_roller, 3);
        lv_obj_align(month_roller, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_size(month_roller, 50, 97);
        lv_roller_set_selected(month_roller, timeinfo->tm_mon, LV_ANIM_ON);
        lv_obj_t *day_roller = lv_roller_create(mbox);
        const char *day_options = "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                                  "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                                  "\n25\n26\n27\n28\n29\n30\n31";
        lv_roller_set_options(day_roller, day_options, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(day_roller, 3);
        lv_obj_align(day_roller, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_size(day_roller, 50, 97);
        lv_roller_set_selected(day_roller, timeinfo->tm_mday-1, LV_ANIM_ON);
    } else {
        lv_obj_t *hour_roller = lv_roller_create(mbox);
        const char *hour_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                                   "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
        lv_roller_set_options(hour_roller, hour_options, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(hour_roller, 3);
        lv_obj_align(hour_roller, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_size(hour_roller, 50, 97);
        lv_roller_set_selected(hour_roller, timeinfo->tm_hour, LV_ANIM_ON);
        lv_obj_t *min_roller = lv_roller_create(mbox);
        const char *min_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                                  "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                                  "\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36"
                                  "\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48"
                                  "\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
        lv_roller_set_options(min_roller, min_options, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(min_roller, 3);
        lv_obj_align(min_roller, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_size(min_roller, 50, 97);
        lv_roller_set_selected(min_roller, timeinfo->tm_min, LV_ANIM_ON);
    }

    lv_obj_t *btn_save = lv_btn_create(mbox);
    lv_obj_add_event_cb(btn_save, msgbox_event_cb, LV_EVENT_CLICKED, (void *)target);
    lv_obj_align(btn_save, LV_ALIGN_TOP_MID, -40, 15);

    lv_obj_t * label = lv_label_create(btn_save);
    lv_obj_set_style_text_font(label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label, "保存");
    lv_obj_set_style_bg_color(btn_save, lv_color_hex(0xB1B2FF), 0);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_center(label);
}

/**
 * @brief 消息框事件回调函数
 * @param e 事件指针
 */
static void msgbox_event_cb(lv_event_t *e)
{
    struct timeval new_time;
    const lv_event_code_t code = lv_event_get_code(e);
    const lv_obj_t *obj = lv_event_get_user_data(e);
    const lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *mbox = lv_obj_get_parent(target);
    lvgl_port_lock(0);
    gettimeofday(&timeval, NULL);
    timeinfo = localtime(&timeval.tv_sec);

    if(code == LV_EVENT_CLICKED && obj == time_app->date.contanier) {
        const lv_obj_t *year_roller = lv_obj_get_child(mbox, 3);
        const lv_obj_t *month_roller = lv_obj_get_child(mbox, 4);
        const lv_obj_t *day_roller = lv_obj_get_child(mbox, 5);
        timeinfo->tm_year = lv_roller_get_selected(year_roller) + 120;
        timeinfo->tm_mon = lv_roller_get_selected(month_roller);
        timeinfo->tm_mday = lv_roller_get_selected(day_roller)+1;
    } else if (code == LV_EVENT_CLICKED && obj == time_app->time.contanier) {
        const lv_obj_t *hour_roller = lv_obj_get_child(mbox, 3);
        const lv_obj_t *min_roller = lv_obj_get_child(mbox, 4);
        timeinfo->tm_hour = lv_roller_get_selected(hour_roller);
        timeinfo->tm_min = lv_roller_get_selected(min_roller);
        timeinfo->tm_sec = 0;
    }
    new_time.tv_sec = mktime(timeinfo);
    if (settimeofday(&new_time, NULL) == -1) {
        LV_LOG_USER("settimeofday error");
        get_time_from_nvs();
    }
    LV_LOG_USER("Time and date has been successfully set!\n");
    lvgl_port_unlock();

    char time_str[24], data_str[39];
    sprintf(time_str, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    lv_label_set_text_fmt(time_app->time.label2, "%s", time_str);
    sprintf(data_str, "%04d年%02d月%02d日", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
    lv_label_set_text_fmt(time_app->date.label2, "%s", data_str);

    set_time_to_nvs();
    lv_msgbox_close(mbox);
}

/**
 * @brief UI返回事件回调函数
 * @param e 事件指针
 */
static void ui_event_return(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    options_screen_t *options_screen = lv_event_get_user_data(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->app == TIME) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_label_set_text(options_screen->label.top, "设 置");
        lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
        lv_obj_load_anim(options_screen->list, time_app->contanier, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);

        lv_obj_del(time_app->contanier);
        options_screen->app = NO_APP;
        free(time_app);
        time_app = NULL;
        lv_obj_clear_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
    }
}