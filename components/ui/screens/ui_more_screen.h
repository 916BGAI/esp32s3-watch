#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *contanier;
    lv_obj_t *icon;

    struct {
        lv_obj_t *cpu;
        lv_obj_t *mem;
        lv_obj_t *flash;
        lv_obj_t *resolution;

        struct {
            lv_obj_t *espidf;
            lv_obj_t *freertos;
            lv_obj_t *lvgl;
            lv_obj_t *soft;
        } version;

    } label;

} more_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif