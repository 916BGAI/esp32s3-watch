#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;

    struct {
        lv_timer_t *time;
        lv_timer_t *calibrate;
    } timer;

    struct {
        lv_obj_t *time;
        lv_obj_t *date;
    } tile;

    struct {
        lv_obj_t *time;
        lv_obj_t *date;
    } label;

    lv_obj_t *clock_contanier;
    lv_obj_t * meter;
} clock_screen_t;

void ui_event_menu_screen(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif