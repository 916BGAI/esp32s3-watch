#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

enum options_app {
    NO_APP,
    Brightness,
    WiFi,
    USB,
};

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *list;
    lv_obj_t *top_contanier;
    lv_style_t style;
    enum options_app app;

    struct {
        lv_obj_t *top;
        lv_obj_t *brightness;
        lv_obj_t *wifi;
        lv_obj_t *usb;
    } label;

    lv_obj_t * ret_button;

} options_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif