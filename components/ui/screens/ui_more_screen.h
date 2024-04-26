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
    } label;

} more_screen_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif