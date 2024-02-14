#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "ui_clock_screen.h"

extern clock_screen_t clock_screen;;
void ui_clock_screen_init(void);

extern lv_obj_t *ui_menu_screen;
void ui_menu_screen_init(void);

void ui_options_screen_init(void);

LV_IMG_DECLARE(ui_img_image);
LV_IMG_DECLARE(ui_img_options);
LV_IMG_DECLARE(ui_img_more);
LV_IMG_DECLARE(ui_img_weather);
LV_IMG_DECLARE(ui_img_clock);

LV_FONT_DECLARE(ui_font_sarasa16);
LV_FONT_DECLARE(ui_font_sarasa18);

#define UI_SYMBOL_WIFI            "\xEE\x9A\xA4"
#define UI_SYMBOL_BRIGHTNESS      "\xEF\x82\x8F"

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif