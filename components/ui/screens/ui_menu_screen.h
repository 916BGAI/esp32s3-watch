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

void app_reg(app_info_t *app_info);
void ui_event_clock_screen(lv_event_t *e);
void ui_event_options_screen(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif