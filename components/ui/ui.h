#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_main_screen
extern lv_obj_t *ui_main_screen;
extern lv_obj_t *ui_image1;
extern lv_obj_t *ui_button1;
extern lv_obj_t *ui_label1;
void ui_main_screen_init(void);
void ui_event_main_screen(lv_event_t *e);
void ui_event_button1(lv_event_t *e);

// SCREEN: ui_menu_screen
extern lv_obj_t *ui_menu_screen;
void ui_menu_screen_init(void);
void ui_event_menu_screen(lv_event_t *e);

// SCREEN: ui_app_options_screen
void app_about(void *user_date);

LV_IMG_DECLARE(ui_img_image_png); // assets\image.png
LV_IMG_DECLARE(ui_img_options_png); // assets\options.png
LV_IMG_DECLARE(ui_img_more_png); // assets\more.png
LV_IMG_DECLARE(ui_img_weather_png); // assets\weather.png
LV_IMG_DECLARE(ui_img_clock_png); // assets\clock.png

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif