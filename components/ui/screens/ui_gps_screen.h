/**
 * @file ui_gps_screen.c
 * @brief GPS屏幕界面功能头文件
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_timer_t *timer;

    struct {
        lv_obj_t *data;
        lv_obj_t *time;
        lv_obj_t *latitude;
        lv_obj_t *longitude;
        lv_obj_t *altitude;
        lv_obj_t *speed;
    } label;

} gps_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif