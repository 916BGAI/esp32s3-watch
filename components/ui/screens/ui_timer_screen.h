#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;

    struct {
        lv_obj_t *hour;
        lv_obj_t *min;
        lv_obj_t *sec;
    } roller;

} timer_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif