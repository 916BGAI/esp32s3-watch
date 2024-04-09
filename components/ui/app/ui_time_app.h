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
        lv_obj_t *label2;
    } date;

    struct {
        lv_obj_t *contanier;
        lv_obj_t *label;
        lv_obj_t *label2;
    } time;

} time_app_t;

void time_event_callback(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif