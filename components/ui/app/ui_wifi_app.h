#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t *contanier;

    struct {
        lv_obj_t *contanier;
        lv_obj_t *label;
        lv_obj_t *btn;
    } sw;

    struct {
        lv_obj_t *contanier;
        lv_obj_t *label;
    } saved;

    lv_obj_t *text1;
    lv_obj_t *text2;

} wifi_app_t;

void wifi_event_callback(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif