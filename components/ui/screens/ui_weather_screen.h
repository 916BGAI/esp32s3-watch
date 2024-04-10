#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
} weather_screen_t;

typedef enum {
    LOCATION_NUM_SHANGHAI,
    LOCATION_NUM_MAX,
} location_num_t;

typedef struct {
    char temp;
    char icon_code;
    char humid;
    char *describe;
} weather_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif