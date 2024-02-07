#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"

extern lv_obj_t *ui_main_screen;
void ui_main_screen_init(void);

extern lv_obj_t *ui_menu_screen;
void ui_menu_screen_init(void);

void ui_options_screen_init(void);

LV_IMG_DECLARE(ui_img_image_png);
LV_IMG_DECLARE(ui_img_options_png);
LV_IMG_DECLARE(ui_img_more_png);
LV_IMG_DECLARE(ui_img_weather_png);
LV_IMG_DECLARE(ui_img_clock_png);

LV_FONT_DECLARE(ui_font_sarasa16);
LV_FONT_DECLARE(ui_font_sarasa18);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif