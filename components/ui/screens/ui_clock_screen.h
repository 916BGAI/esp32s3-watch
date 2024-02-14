#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;

    struct _timer {
        lv_timer_t *calibrate;
        lv_timer_t *clock;
    } timer;

    struct _tile {
        lv_obj_t *time;
        lv_obj_t *date;
    } tile;

    struct _label {
        lv_obj_t *time;
        lv_obj_t *date;
    } label;

    lv_obj_t *clock_contanier;
    lv_style_t style;
    bool lock;
} clock_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif