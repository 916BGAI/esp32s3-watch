/**
 * @file clock_screen.h
 * @brief 定义时钟屏幕结构体。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief 表示时钟屏幕的结构体。
 */
typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;
    lv_obj_t *clock_contanier;
    lv_obj_t *meter;

    struct {
        lv_obj_t *tile;
        lv_obj_t *label;
    } time;

    struct {
        lv_obj_t *tile;
        lv_obj_t *label;
    } date;

    struct {
        lv_timer_t *time;
        lv_timer_t *calibrate;
    } timer;

} clock_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif