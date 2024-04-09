#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void ui_clock_screen_init(void);

void ui_menu_screen_init(void);

void ui_options_screen_init(void);

LV_IMG_DECLARE(ui_img_options);
LV_IMG_DECLARE(ui_img_more);
LV_IMG_DECLARE(ui_img_weather);
LV_IMG_DECLARE(ui_img_clock);
LV_IMG_DECLARE(ui_img_hand_hour);
LV_IMG_DECLARE(ui_img_hand_min);
LV_IMG_DECLARE(ui_img_hand_sec);

extern lv_font_t *SarasaMonoR_16;
extern lv_font_t *SarasaMonoR_18;
extern lv_font_t *SarasaMonoB_18;

#define UI_SYMBOL_USB             "\xEE\x9F\x85"
#define UI_SYMBOL_WIFI            "\xEE\x9A\xA4"
#define UI_SYMBOL_BRIGHTNESS      "\xEF\x82\x8F"
#define UI_SYMBOL_CLOCK           "\xEE\x9D\x8F"

void ui_init(void);

static inline void lv_font_init()
{
    SarasaMonoR_16 = lv_font_load("A:fonts/SarasaMonoSC_regular_16.bin");
    SarasaMonoR_18 = lv_font_load("A:fonts/SarasaMonoSC_regular_18.bin");
    SarasaMonoB_18 = lv_font_load("A:fonts/SarasaMonoSC_bold_18.bin");
}

#ifdef __cplusplus
} /*extern "C"*/
#endif