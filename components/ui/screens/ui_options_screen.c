#include "../ui.h"
#include "ui_helpers.h"
#include "ui_menu_screen.h"
#include "ui_brightness_app.h"
#include "ui_wifi_app.h"
#include "ui_options_screen.h"
#include "esp_lvgl_port.h"
#include "display.h"

static options_screen_t *options_screen;

static void return_save_button_event_callback(lv_event_t *e);
static void ui_event_options_screen(lv_event_t *e);

void ui_options_screen_init(void)
{
    options_screen = malloc(sizeof(options_screen_t));
    options_screen->app = NO_APP;

    options_screen->screen = lv_obj_create(NULL);

    options_screen->top_contanier = lv_obj_create(options_screen->screen);
    lv_obj_set_style_text_font(options_screen->screen, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(options_screen->top_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(options_screen->top_contanier, 240, 40);
    lv_obj_align(options_screen->top_contanier, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(options_screen->top_contanier, lv_color_hex(0xEFF0F1), 0);

    options_screen->label.top = lv_label_create(options_screen->top_contanier);
    lv_obj_set_style_text_font(options_screen->label.top, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_screen->label.top, "设 置");
    lv_obj_align(options_screen->label.top, LV_ALIGN_CENTER, 0, 0);

    options_screen->button.ret = lv_btn_create(options_screen->top_contanier);
    lv_obj_t *return_button_label = lv_label_create(options_screen->button.ret);
    lv_obj_set_size(options_screen->button.ret, 39, 27);
    lv_obj_align(options_screen->button.ret, LV_ALIGN_LEFT_MID, -25, 0);
    lv_label_set_text(return_button_label, "返回");
    lv_obj_center(return_button_label);
    lv_obj_add_event_cb(options_screen->button.ret, return_save_button_event_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(options_screen->button.ret, LV_OBJ_FLAG_HIDDEN);

    options_screen->button.save = lv_btn_create(options_screen->top_contanier);
    lv_obj_t *save_button_label = lv_label_create(options_screen->button.save);
    lv_obj_set_size(options_screen->button.save, 39, 27);
    lv_obj_align(options_screen->button.save, LV_ALIGN_RIGHT_MID, 25, 0);
    lv_label_set_text(save_button_label, "保存");
    lv_obj_center(save_button_label);
    lv_obj_add_event_cb(options_screen->button.save, return_save_button_event_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(options_screen->button.save, LV_OBJ_FLAG_HIDDEN);

    options_screen->list = lv_list_create(options_screen->screen);
    lv_obj_set_size(options_screen->list, 240, 240);
    lv_obj_align(options_screen->list, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_BRIGHTNESS, "屏幕亮度");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, options_screen);
    options_screen->label.brightness = lv_label_create(btn);
    lv_obj_set_style_text_font(options_screen->label.brightness, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(options_screen->label.brightness, "%lu%%", display_brightness_get());

    btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_WIFI, "Wi-Fi");
    lv_obj_add_event_cb(btn, wifi_event_callback, LV_EVENT_CLICKED, options_screen);
    options_screen->label.wifi = lv_label_create(btn);
    lv_obj_set_style_text_font(options_screen->label.wifi, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_screen->label.wifi, "关");

    lv_style_init(&options_screen->style);
    lv_style_set_border_width(&options_screen->style, 0);
    lv_obj_add_style(options_screen->list, &options_screen->style, 0);

    lv_obj_add_event_cb(options_screen->screen, ui_event_options_screen, LV_EVENT_ALL, options_screen);

    lv_scr_load_anim(options_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

extern brightness_app_t *brightness_app;

void return_save_button_event_callback(lv_event_t *e)
{
    const lv_obj_t *target = lv_event_get_target(e);

    if (target == options_screen->button.save) {
        switch (options_screen->app) {
        case Brightness:
            const uint32_t brightness_percent = lv_slider_get_value(brightness_app->slider);
            lvgl_port_lock(0);
            display_brightness_set(brightness_percent);
            lv_label_set_text_fmt(options_screen->label.brightness, "%lu%%", brightness_percent);
            lvgl_port_unlock();
            break;
        case WiFi:

            break;
        default:
            break;
        }
    }

    lv_obj_add_flag(options_screen->button.ret, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(options_screen->button.save, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(options_screen->label.top, "设 置");
    lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
    lv_obj_load_anim(options_screen->list, brightness_app->contanier, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);
    lv_style_reset(&brightness_app->style_main);
    lv_style_reset(&brightness_app->style_indicator);
    lv_style_reset(&brightness_app->style_pressed_color);
    lv_style_reset(&brightness_app->style_knob);
    lv_style_reset(&brightness_app->style_pressed_color);
    lv_obj_del(brightness_app->contanier);
    options_screen->app = NO_APP;
    free(brightness_app);
    lv_obj_clear_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
}

extern menu_screen_t *menu_screen;
void ui_event_options_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->app == NO_APP) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        free(options_screen);
    }
}