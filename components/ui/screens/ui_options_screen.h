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
    TIME,
};

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *list;
    lv_obj_t *top_contanier;
    enum options_app app;

    struct {
        lv_obj_t *top;
        lv_obj_t *brightness;
        lv_obj_t *wifi;
        lv_obj_t *usb;
    } label;

} options_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif