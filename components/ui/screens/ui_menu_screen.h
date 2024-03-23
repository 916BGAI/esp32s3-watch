#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *tv;
    lv_obj_t *tile;
} menu_screen_t;

typedef struct {
    uint32_t id;
    char *name;
    const lv_img_dsc_t *img_src;
} app_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif