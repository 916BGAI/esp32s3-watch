/**
 * @file ui_options_screen.h
 * @brief 实现了选项界面的UI模块头文件。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief 表示不同选项应用程序的枚举。
 */
enum options_app {
    NO_APP,             /**< 未选择任何应用程序 */
    Brightness,         /**< 亮度应用程序 */
    WiFi,               /**< WiFi 应用程序 */
    USB,                /**< USB 应用程序 */
    TIME,               /**< 时间应用程序 */
};

/**
 * @brief 表示选项屏幕的结构体。
 */
typedef struct {
    lv_obj_t *screen;
    lv_obj_t *list;
    lv_obj_t *top_contanier;
    enum options_app app;

    struct {
        lv_obj_t *top;
        lv_obj_t *brightness;
        lv_obj_t *wifi;
        lv_obj_t *usb;
    } label;

} options_screen_t;             /**< 选项屏幕结构体 */

#ifdef __cplusplus
} /*extern "C"*/
#endif