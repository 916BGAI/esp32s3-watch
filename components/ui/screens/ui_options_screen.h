#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct _options_screen_t {
    lv_obj_t *screen;
    lv_obj_t *list;
    lv_obj_t *top_contanier;
    lv_obj_t *top_label;
    lv_obj_t *return_button;
    lv_obj_t *save_button;
    lv_style_t style;
    bool mutex;
} options_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif