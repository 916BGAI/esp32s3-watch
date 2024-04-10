#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_options_screen.h"
#include "ui_usb_app.h"
#include "usb.h"

#include <esp_lvgl_port.h>

usb_app_t *usb_app;
static bool usb_status = false;

static void sw_event_cb(lv_event_t *e);
static void ui_event_return(lv_event_t *e);

void usb_event_callback(lv_event_t *e)
{
    usb_app = malloc(sizeof(usb_app_t));
    options_screen_t *options_screen = lv_event_get_user_data(e);

    options_screen->app = USB;

    lv_obj_add_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
    usb_app->contanier = lv_obj_create(options_screen->screen);
    lv_obj_set_style_border_width(usb_app->contanier, 0, 0);
    lv_obj_set_size(usb_app->contanier, 240, 240);
    lv_obj_align(usb_app->contanier, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_load_anim(usb_app->contanier, options_screen->list, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 100);
    lv_label_set_text(options_screen->label.top, "USB 调试");
    lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
    lv_obj_add_event_cb(options_screen->screen, ui_event_return, LV_EVENT_ALL, options_screen);

    /* Switch of USB */
    usb_app->sw.contanier = lv_obj_create(usb_app->contanier);
    lv_obj_set_size(usb_app->sw.contanier, 210, 53);
    lv_obj_align(usb_app->sw.contanier, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(usb_app->sw.contanier, 0, 0);
    lv_obj_clear_flag(usb_app->sw.contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(usb_app->sw.contanier, lv_color_hex(0xD5DAF9), 0);
    usb_app->sw.label = lv_label_create(usb_app->sw.contanier);
    lv_obj_set_style_text_font(usb_app->sw.label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(usb_app->sw.label, LV_ALIGN_LEFT_MID, 3, 0);
    lv_label_set_text(usb_app->sw.label, "MSC 设备");

    usb_app->sw.btn = lv_switch_create(usb_app->sw.contanier);
    lv_obj_set_style_bg_color(usb_app->sw.btn, lv_color_hex(0xB1B2FF), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_size(usb_app->sw.btn, 46, 29);
    lv_obj_align(usb_app->sw.btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(usb_app->sw.btn, sw_event_cb, LV_EVENT_ALL, options_screen);

    if (usb_status) {
        lv_obj_add_state(usb_app->sw.btn, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(usb_app->sw.btn, LV_STATE_CHECKED);
    }
}

static void sw_event_cb(lv_event_t *e)
{
    options_screen_t *options_screen = lv_event_get_user_data(e);
    const lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        usb_status = true;
        lv_label_set_text(options_screen->label.usb, "开");
        lvgl_port_lock(0);
        usb_init();
        lvgl_port_unlock();
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        usb_status = false;
        lv_label_set_text(options_screen->label.usb, "关");
        lvgl_port_lock(0);
        usb_deinit();
        lvgl_port_unlock();
    }
}

static void ui_event_return(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    options_screen_t *options_screen = lv_event_get_user_data(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->app == USB) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_label_set_text(options_screen->label.top, "设 置");
        lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
        lv_obj_load_anim(options_screen->list, usb_app->contanier, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);

        lv_obj_del(usb_app->contanier);
        options_screen->app = NO_APP;
        free(usb_app);
        lv_obj_clear_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
    }
}