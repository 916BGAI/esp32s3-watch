#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_mainScreen
void ui_mainScreen_screen_init(void);
void ui_event_mainScreen(lv_event_t * e);
extern lv_obj_t * ui_mainScreen;
extern lv_obj_t * ui_Image1;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Label1;
// SCREEN: ui_menuScreen
void ui_menuScreen_screen_init(void);
void ui_event_menuScreen(lv_event_t * e);
extern lv_obj_t * ui_menuScreen;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_image_png);    // assets\image.png
LV_IMG_DECLARE(ui_img_options_png);    // assets\options.png
LV_IMG_DECLARE(ui_img_more_png);    // assets\more.png
LV_IMG_DECLARE(ui_img_weather_png);    // assets\weather.png
LV_IMG_DECLARE(ui_img_clock_png);    // assets\clock.png



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
