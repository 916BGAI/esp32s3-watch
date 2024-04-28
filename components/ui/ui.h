/**
 * @file ui.h
 * @brief 该文件包含与用户界面相关的函数和声明。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void ui_clock_screen_init(void);
void ui_menu_screen_init(void);
void ui_options_screen_init(void);
void ui_weather_screen_init(void);
void ui_timer_screen_init(void);
void ui_gps_screen_init(void);
void ui_more_screen_init(void);

LV_IMG_DECLARE(ui_img_hand_hour);
LV_IMG_DECLARE(ui_img_hand_min);
LV_IMG_DECLARE(ui_img_hand_sec);

extern lv_font_t *SarasaMonoR_16;
extern lv_font_t *SarasaMonoR_18;
extern lv_font_t *SarasaMonoB_18;
extern lv_font_t *SarasaMonoB_30;

#define UI_SYMBOL_USB             "\xEE\x9F\x85"
#define UI_SYMBOL_WIFI            "\xEE\x9A\xA4"
#define UI_SYMBOL_BRIGHTNESS      "\xEF\x82\x8F"
#define UI_SYMBOL_CLOCK           "\xEE\x9D\x8F"
#define UI_SYMBOL_CPU             "\xEE\x98\x8C"
#define UI_SYMBOL_MEM             "\xEE\x98\xA7"
#define UI_SYMBOL_FLASH           "\xEE\x9B\xAC"
#define UI_SYMBOL_RESOLUTION      "\xEE\x99\x82"
#define UI_SYMBOL_ESPIDF          "\xEE\x9A\x83"
#define UI_SYMBOL_FREERTOS        "\xEE\x9C\x85"
#define UI_SYMBOL_VERSION         "\xEE\x9B\xBA"

void ui_init(void);

/**
 * @brief 初始化 Sarasa Mono 字体。
 */
static inline void lv_font_init()
{
    SarasaMonoR_16 = lv_font_load("A:fonts/SarasaMonoSC_regular_16.bin");
    SarasaMonoR_18 = lv_font_load("A:fonts/SarasaMonoSC_regular_18.bin");
    SarasaMonoB_18 = lv_font_load("A:fonts/SarasaMonoSC_bold_18.bin");
    SarasaMonoB_30 = lv_font_load("A:fonts/SarasaMonoSC_bold_30.bin");
}

#ifdef __cplusplus
} /*extern "C"*/
#endif