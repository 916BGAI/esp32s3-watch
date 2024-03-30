#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_options_screen.h"
#include "ui_wifi_app.h"
#include "wifi.h"

wifi_app_t *wifi_app;

static void sw_event_cb(lv_event_t *e);
static void saved_event_cb(lv_event_t *e);
static void saved_msgbox_event_cb(lv_event_t *e);
static void ui_event_return(lv_event_t *e);

void wifi_event_callback(lv_event_t *e)
{
    wifi_app = malloc(sizeof(wifi_app_t));
    options_screen_t *options_screen = e->user_data;

    options_screen->app = WiFi;

    lv_obj_add_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
    wifi_app->contanier = lv_obj_create(options_screen->screen);
    lv_obj_add_style(wifi_app->contanier, &options_screen->style, 0);
    lv_obj_set_size(wifi_app->contanier, 240, 240);
    lv_obj_align(wifi_app->contanier, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_load_anim(wifi_app->contanier, options_screen->list, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 100);
    lv_label_set_text(options_screen->label.top, "Wi-Fi");
    lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
    lv_obj_add_event_cb(options_screen->screen, ui_event_return, LV_EVENT_ALL, options_screen);

    /* Switch of Wi-Fi */
    wifi_app->sw.contanier = lv_obj_create(wifi_app->contanier);
    lv_obj_set_size(wifi_app->sw.contanier, 210, 53);
    lv_obj_align(wifi_app->sw.contanier, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(wifi_app->sw.contanier, &options_screen->style, 0);
    lv_obj_clear_flag(wifi_app->sw.contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(wifi_app->sw.contanier, lv_color_hex(0xD5DAF9), 0);
    wifi_app->sw.label = lv_label_create(wifi_app->sw.contanier);
    lv_obj_set_style_text_font(wifi_app->sw.label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(wifi_app->sw.label, LV_ALIGN_LEFT_MID, 3, 0);

    wifi_app->sw.btn = lv_switch_create(wifi_app->sw.contanier);
    lv_obj_set_style_bg_color(wifi_app->sw.btn, lv_color_hex(0xB1B2FF), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_size(wifi_app->sw.btn, 46, 29);
    lv_obj_align(wifi_app->sw.btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(wifi_app->sw.btn, sw_event_cb, LV_EVENT_ALL, options_screen);

    wifi_app->text1 = lv_label_create(wifi_app->contanier);
    lv_obj_set_style_text_font(wifi_app->text1, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(wifi_app->text1, "若要查看可用网络，请打开\nWi-Fi");
    lv_obj_set_style_text_line_space(wifi_app->text1, 3, 0);
    lv_obj_align_to(wifi_app->text1, wifi_app->sw.contanier, LV_ALIGN_OUT_BOTTOM_LEFT, 7, 11);

    wifi_app->text2 = lv_label_create(wifi_app->contanier);
    lv_obj_set_style_text_font(wifi_app->text2, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(wifi_app->text2, "已保存的网络");
    lv_obj_set_style_text_line_space(wifi_app->text2, 3, 0);
    lv_obj_align_to(wifi_app->text2, wifi_app->sw.contanier, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    /* Saved Wi-Fi */
    wifi_app->saved.contanier = lv_obj_create(wifi_app->contanier);
    lv_obj_set_size(wifi_app->saved.contanier, 210, 53);
    lv_obj_align_to(wifi_app->saved.contanier, wifi_app->text2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_style(wifi_app->saved.contanier, &options_screen->style, 0);
    lv_obj_clear_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(wifi_app->saved.contanier, lv_color_hex(0xD5DAF9), 0);
    wifi_app->saved.label = lv_label_create(wifi_app->saved.contanier);
    lv_obj_set_style_text_font(wifi_app->saved.label, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(wifi_app->saved.label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(wifi_app->saved.contanier, saved_event_cb, LV_EVENT_ALL, NULL);

    if (wifi_info_get_status() == true) {
        lv_obj_add_state(wifi_app->sw.btn, LV_STATE_CHECKED);
        lv_label_set_text(wifi_app->sw.label, "开");
        lv_obj_add_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_state(wifi_app->sw.btn, LV_STATE_CHECKED);
        lv_label_set_text(wifi_app->sw.label, "关");
        lv_obj_add_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);
    }
}

static void sw_event_cb(lv_event_t *e)
{
    options_screen_t *options_screen = e->user_data;
    const lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        lv_label_set_text(wifi_app->sw.label, "开");
        lv_label_set_text(options_screen->label.wifi, "开");
        wifi_info_set_status(true);
        lv_obj_add_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);

    } else if (code == LV_EVENT_VALUE_CHANGED) {
        lv_label_set_text(wifi_app->sw.label, "关");
        lv_label_set_text(options_screen->label.wifi, "关");
        wifi_info_set_status(false);
        lv_obj_clear_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);
    }
}

static void saved_event_cb(lv_event_t *e)
{
    const lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_LONG_PRESSED) {
        static const char * btns[] ={"Apply", "Close", ""};
        lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Hello", "This is a message box with two buttons.", btns, true);
        lv_obj_add_event_cb(mbox1, saved_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(mbox1);
    }
}

static void saved_msgbox_event_cb(lv_event_t *e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

static void ui_event_return(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    options_screen_t *options_screen = e->user_data;

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->app == WiFi) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_label_set_text(options_screen->label.top, "设 置");
        lv_obj_load_anim(options_screen->label.top, options_screen->label.top, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
        lv_obj_load_anim(options_screen->list, wifi_app->contanier, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);

        lv_obj_del(wifi_app->contanier);
        options_screen->app = NO_APP;
        free(wifi_app);
        lv_obj_clear_flag(options_screen->list, LV_OBJ_FLAG_HIDDEN);
        wifi_info_save_to_nvs();
    }
}