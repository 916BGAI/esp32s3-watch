#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ui_event_main_screen(lv_event_t *e);
void ui_event_button1(lv_event_t *e);
void ui_event_menu_screen(lv_event_t *e);
void led_event_callback(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif