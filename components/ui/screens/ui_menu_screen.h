/**
 * @file ui_menu_screen.h
 * @brief 定义菜单屏幕结构体和应用程序信息结构体。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/** 菜单屏幕结构体 */
typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;
    lv_obj_t *tile;
} menu_screen_t;

/** 应用程序信息结构体 */
typedef struct {
    uint32_t id;
    char *name;
    char *img_src;
} app_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif