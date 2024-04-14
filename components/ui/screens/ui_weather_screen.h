#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

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

typedef enum {
    LOCATION_NUM_ZHENGZHOU,
    LOCATION_NUM_BEIJING,
    LOCATION_NUM_NEWYORK,
    LOCATION_NUM_MAX,
} location_num_t;

typedef struct {
    int  temp;
    int  icon_code;
    int  humidity;
    int  windScale;
    char *obsTime;
    char *describe;
    char *windDir;
} weather_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif