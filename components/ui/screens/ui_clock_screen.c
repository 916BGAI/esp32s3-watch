/**
 * @file ui_clock_screen.c
 * @brief 实现时钟屏幕UI模块。
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "nvs_flash.h"
#include "../main/include/sntp.h"
#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_clock_screen.h"

clock_screen_t clock_screen;                /**< 时钟屏幕对象 */
static struct tm *timeinfo;

static lv_meter_indicator_t * indic_hour;
static lv_meter_indicator_t * indic_min;
static lv_meter_indicator_t * indic_sec;

static void ui_event_clock_screen(lv_event_t *e);
static void ui_time_update(lv_timer_t *timer);
static void ui_time_calibrate(lv_timer_t *timer);

/**
 * @brief 初始化时钟屏幕UI模块。
 */
void ui_clock_screen_init(void)
{
    // 创建时钟屏幕对象
    clock_screen.screen = lv_obj_create(NULL);
    lv_obj_clear_flag(clock_screen.screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(clock_screen.screen, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建时间和日期视图
    clock_screen.tv = lv_tileview_create(clock_screen.screen);
    lv_obj_set_size(clock_screen.tv, 240, 40);
    lv_obj_align(clock_screen.tv, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_bg_opa(clock_screen.tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(clock_screen.tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_font(clock_screen.tv, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    clock_screen.time.tile = lv_tileview_add_tile(clock_screen.tv, 0, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    clock_screen.time.label = lv_label_create(clock_screen.time.tile);
    lv_obj_center(clock_screen.time.label);
    clock_screen.date.tile = lv_tileview_add_tile(clock_screen.tv, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    clock_screen.date.label = lv_label_create(clock_screen.date.tile);
    lv_obj_center(clock_screen.date.label);

    // 创建时钟容器和表盘
    clock_screen.clock_contanier = lv_obj_create(clock_screen.screen);
    lv_obj_set_style_border_width(clock_screen.clock_contanier, 0, LV_PART_MAIN);
    lv_obj_clear_flag(clock_screen.clock_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(clock_screen.clock_contanier, 220, 220);
    lv_obj_align(clock_screen.clock_contanier, LV_ALIGN_TOP_MID, 0, 50);

    clock_screen.meter = lv_meter_create(clock_screen.clock_contanier);
    lv_obj_set_size(clock_screen.meter, 210, 210);
    lv_obj_center(clock_screen.meter);

    // 设置表盘刻度
    lv_meter_scale_t * scale_min = lv_meter_add_scale(clock_screen.meter);
    lv_meter_set_scale_ticks(clock_screen.meter, scale_min, 61, 1, 7, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(clock_screen.meter, scale_min, 0, 60, 360, 270);

    /*Create another scale for the hours. It's only visual and contains only major ticks*/
    lv_meter_scale_t * scale_hour = lv_meter_add_scale(clock_screen.meter);
    lv_meter_set_scale_ticks(clock_screen.meter, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));               /*12 ticks*/
    lv_meter_set_scale_major_ticks(clock_screen.meter, scale_hour, 1, 2, 10, lv_color_black(), 10);    /*Every tick is major*/
    lv_meter_set_scale_range(clock_screen.meter, scale_hour, 1, 12, 330, 300);       /*[1..12] values in an almost full circle*/

    // 添加指针
    indic_hour = lv_meter_add_needle_img(clock_screen.meter, scale_min, &ui_img_hand_hour, 5, 5);
    indic_min = lv_meter_add_needle_img(clock_screen.meter, scale_min, &ui_img_hand_min, 5, 5);
    indic_sec = lv_meter_add_needle_img(clock_screen.meter, scale_min, &ui_img_hand_sec, 5, 5);

    // 初始化时钟
    ESP_ERROR_CHECK(get_time_from_nvs());

    // 创建时钟更新定时器
    clock_screen.timer.time = lv_timer_create(ui_time_update, 1000, NULL);
    clock_screen.timer.calibrate = lv_timer_create(ui_time_calibrate, 24*60*60*1000, NULL);
    ui_time_update(NULL);

    // 添加时钟屏幕事件回调
    lv_obj_add_event_cb(clock_screen.screen, ui_event_clock_screen, LV_EVENT_ALL, NULL);

    // 加载时钟屏幕
    lv_disp_load_scr(clock_screen.screen);
}

extern menu_screen_t *menu_screen;
/**
 * @brief 时钟屏幕UI的事件处理程序。
 * 
 * @param e 事件对象
 */
void ui_event_clock_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
    }
}

/**
 * @brief 更新时钟屏幕上的时间。
 * 
 * @param timer 定时器对象
 */
void ui_time_update(lv_timer_t *timer)
{
    static struct timeval timeval = {
        .tv_sec = 0,
        .tv_usec = 0,
    };

    // 获取当前时间
    gettimeofday(&timeval, NULL);
    timeinfo = localtime(&timeval.tv_sec);
    lv_label_set_text_fmt(clock_screen.time.label, "%02d : %02d", timeinfo->tm_hour, timeinfo->tm_min);
    lv_label_set_text_fmt(clock_screen.date.label, "%04d 年 %02d 月 %02d 日", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);

    // 更新指针位置
    lv_meter_set_indicator_end_value(clock_screen.meter, indic_hour,
                                     (timeinfo->tm_hour <= 12 ? timeinfo->tm_hour : timeinfo->tm_hour - 12) * 5 + timeinfo->tm_min/12);
    lv_meter_set_indicator_end_value(clock_screen.meter, indic_min, timeinfo->tm_min);
    lv_meter_set_indicator_end_value(clock_screen.meter, indic_sec, timeinfo->tm_sec);
}

/**
 * @brief 定期校准时间。
 * 
 * @param timer 定时器对象
 */
void ui_time_calibrate(lv_timer_t *timer)
{
    time_update();
}