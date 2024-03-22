#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t *contanier;
    lv_obj_t *slider;
    lv_obj_t *slider_label;
    char slider_label_buf[8];
    lv_style_t style_main;
    lv_style_t style_indicator;
    lv_style_t style_knob;
    lv_style_t style_pressed_color;
} brightness_app_t;

void brightness_event_callback(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif