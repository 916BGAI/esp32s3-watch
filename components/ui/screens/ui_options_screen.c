/**
 * @file ui_options_screen.c
 * @brief 实现了选项界面的UI模块。
 */

#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_brightness_app.h"
#include "ui_wifi_app.h"
#include "ui_usb_app.h"
#include "ui_time_app.h"
#include "ui_options_screen.h"
#include "esp_lvgl_port.h"
#include "display.h"
#include "wifi.h"

static options_screen_t *options_screen;                /**< 指向选项界面结构体的指针。 */

static void ui_event_options_screen(lv_event_t *e);

/**
 * @brief 初始化选项界面的UI模块。
 */
void ui_options_screen_init(void)
{
    // 创建选项界面对象
    options_screen = malloc(sizeof(options_screen_t));
    options_screen->app = NO_APP;

    // 创建屏幕对象
    options_screen->screen = lv_obj_create(NULL);

    // 创建顶部容器
    options_screen->top_contanier = lv_obj_create(options_screen->screen);
    lv_obj_set_style_text_font(options_screen->screen, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(options_screen->top_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(options_screen->top_contanier, 240, 40);
    lv_obj_align(options_screen->top_contanier, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(options_screen->top_contanier, lv_color_hex(0xEFF0F1), 0);

    // 创建标题标签
    options_screen->label.top = lv_label_create(options_screen->top_contanier);
    lv_obj_set_style_text_font(options_screen->label.top, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_screen->label.top, "设 置");
    lv_obj_align(options_screen->label.top, LV_ALIGN_CENTER, 0, 0);

    // 创建选项列表
    options_screen->list = lv_list_create(options_screen->screen);
    lv_obj_set_size(options_screen->list, 240, 240);
    lv_obj_align(options_screen->list, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_border_width(options_screen->list, 0, 0);

    // 添加屏幕亮度选项按钮
    lv_obj_t *btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_BRIGHTNESS, "屏幕亮度");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, options_screen);
    options_screen->label.brightness = lv_label_create(btn);
    lv_obj_set_style_text_font(options_screen->label.brightness, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(options_screen->label.brightness, "%lu%%", display_brightness_get());

    // 添加Wi-Fi选项按钮
    btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_WIFI, "Wi-Fi");
    lv_obj_add_event_cb(btn, wifi_event_callback, LV_EVENT_CLICKED, options_screen);
    options_screen->label.wifi = lv_label_create(btn);
    lv_obj_set_style_text_font(options_screen->label.wifi, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    wifi_info_obtain_from_nvs();
    if (wifi_info_get_status() == true) {
        lv_label_set_text(options_screen->label.wifi, "开");
    } else {
        lv_label_set_text(options_screen->label.wifi, "关");
    }

    // 添加日期和时间选项按钮
    btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_CLOCK, "日期和时间");
    lv_obj_add_event_cb(btn, time_event_callback, LV_EVENT_CLICKED, options_screen);

    // 添加USB调试选项按钮
    btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_USB, "USB 调试");
    lv_obj_add_event_cb(btn, usb_event_callback, LV_EVENT_CLICKED, options_screen);
    options_screen->label.usb = lv_label_create(btn);
    lv_obj_set_style_text_font(options_screen->label.usb, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_screen->label.usb, "关");

    // 添加事件处理
    lv_obj_add_event_cb(options_screen->screen, ui_event_options_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(options_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

extern menu_screen_t *menu_screen;
/**
 * @brief 选项界面的事件处理函数。
 * 
 * @param e 指向事件的指针
 */
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
        options_screen = NULL;
    }
}