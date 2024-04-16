#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *label_time;
    lv_timer_t *timer;

    struct {
        lv_obj_t *hour;
        lv_obj_t *min;
        lv_obj_t *sec;
    } roller;

    struct {
        lv_obj_t *start;
        lv_obj_t *pause;
        lv_obj_t *stop;
    } btn;

} timer_screen_t;

typedef struct {
    int hour;
    int min;
    int sec;

    enum {
        STOP = 0,
        PAUSE,
        START,
    } status;

} timer_config_t;

timer_config_t get_timer_config(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif