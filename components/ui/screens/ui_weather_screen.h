/**
 * @file ui_weather_screen.h
 * @brief 天气界面UI头文件
 */


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief 天气界面结构体
 */
typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;
    lv_obj_t *tile1;
    lv_obj_t *tile2;
    lv_obj_t *tile3;

    lv_obj_t *weather_icon;

    struct {
        lv_obj_t *temp;
        lv_obj_t *humidity;
        lv_obj_t *obsTime;
        lv_obj_t *describe;
        lv_obj_t *windScale;
        lv_obj_t *windDir;
    } label;

} weather_screen_t;

/**
 * @brief 地点编号枚举
 */
typedef enum {
    LOCATION_NUM_ZHENGZHOU,
    LOCATION_NUM_BEIJING,
    LOCATION_NUM_NEWYORK,
    LOCATION_NUM_MAX,
} location_num_t;

/**
 * @brief 天气信息结构体
 */
typedef struct {
    int  temp;              /**< 温度 */
    int  icon_code;         /**< 天气图标代码 */
    int  humidity;          /**< 湿度 */
    int  windScale;         /**< 风力等级 */
    char *obsTime;          /**< 观测时间 */
    char *describe;         /**< 天气描述 */
    char *windDir;          /**< 风向 */
} weather_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif