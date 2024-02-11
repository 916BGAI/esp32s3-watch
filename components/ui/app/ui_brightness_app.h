#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void brightness_event_callback(lv_event_t *e);
void return_save_button_event_callback(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif