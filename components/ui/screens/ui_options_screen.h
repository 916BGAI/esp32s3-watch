#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

enum options_app {
    NO_APP,
    Brightness,
    WiFi,
};

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *list;
    lv_obj_t *top_contanier;
    lv_obj_t *top_label;
    lv_obj_t *return_button;
    lv_obj_t *save_button;
    lv_style_t style;
    enum options_app app;
} options_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif