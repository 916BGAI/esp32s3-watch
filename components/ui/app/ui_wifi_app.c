#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_options_screen.h"
#include "ui_wifi_app.h"
#include "esp_wifi.h"
#include "wifi.h"

#include <esp_lvgl_port.h>

wifi_app_t *wifi_app;

static void sw_event_cb(lv_event_t *e);
static void saved_event_cb(lv_event_t *e);
static void saved_msgbox_event_cb(lv_event_t *e);
static void add_event_cb(lv_event_t *e);
static void add_msgbox_event_cb(lv_event_t *e);
static void add_msgbox_ta_event_cb(lv_event_t * e);
static void add_msgbox_dropdown_event_cb(lv_event_t * e);
static void add_msgbox_btn_event_cb(lv_event_t *e);
static void ui_event_return(lv_event_t *e);

void wifi_event_callback(lv_event_t *e)
{
    LV_LOG_USER("free_internal_heap_size = %ldKB", esp_get_free_internal_heap_size() / 1024);
    LV_LOG_USER("free_heap_size = %ldKB", esp_get_free_heap_size() / 1024);
    wifi_app = malloc(sizeof(wifi_app_t));
    options_screen_t *options_screen = lv_event_get_user_data(e);

    options_screen->app = WiFi;
    wifi_info_obtain_from_nvs();

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
    if (strlen((char *)wifi_info_get_ssid()) == 0) {
        lv_label_set_text(wifi_app->saved.label, "无");
    } else if (wifi_get_connected_status(0) == ESP_OK) {
        lv_label_set_text_fmt(wifi_app->saved.label, "%s（已连接）", wifi_info_get_ssid());
    } else {
        lv_label_set_text_fmt(wifi_app->saved.label, "%s", wifi_info_get_ssid());
    }

    lv_obj_align(wifi_app->saved.label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(wifi_app->saved.contanier, saved_event_cb, LV_EVENT_ALL, NULL);

    wifi_app->text3 = lv_label_create(wifi_app->contanier);
    lv_obj_set_style_text_font(wifi_app->text3, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(wifi_app->text3, "可用的网络");
    lv_obj_set_style_text_line_space(wifi_app->text3, 3, 0);
    lv_obj_align_to(wifi_app->text3, wifi_app->saved.contanier, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    /* add Wi-Fi */
    wifi_app->add.contanier = lv_obj_create(wifi_app->contanier);
    lv_obj_set_size(wifi_app->add.contanier, 210, 53);
    lv_obj_align_to(wifi_app->add.contanier, wifi_app->text3, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_style(wifi_app->add.contanier, &options_screen->style, 0);
    lv_obj_clear_flag(wifi_app->add.contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(wifi_app->add.contanier, lv_color_hex(0xD5DAF9), 0);
    wifi_app->add.label = lv_label_create(wifi_app->add.contanier);
    lv_obj_set_style_text_font(wifi_app->add.label, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(wifi_app->add.label, LV_ALIGN_CENTER, -5, 0);
    lv_label_set_text(wifi_app->add.label, LV_SYMBOL_PLUS "   添 加 网 络");
    lv_obj_add_event_cb(wifi_app->add.contanier, add_event_cb, LV_EVENT_ALL, NULL);

    if (wifi_info_get_status() == true) {
        lv_obj_add_state(wifi_app->sw.btn, LV_STATE_CHECKED);
        lv_label_set_text(wifi_app->sw.label, "开");
        lv_obj_add_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_state(wifi_app->sw.btn, LV_STATE_CHECKED);
        lv_label_set_text(wifi_app->sw.label, "关");
        lv_obj_add_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->text3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->add.contanier, LV_OBJ_FLAG_HIDDEN);
    }
}

static void sw_event_cb(lv_event_t *e)
{
    options_screen_t *options_screen = lv_event_get_user_data(e);
    const lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        lv_label_set_text(wifi_app->sw.label, "开");
        lv_label_set_text(options_screen->label.wifi, "开");
        wifi_info_set_status(true);
        lv_obj_add_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->text3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_app->add.contanier, LV_OBJ_FLAG_HIDDEN);

    } else if (code == LV_EVENT_VALUE_CHANGED) {
        lv_label_set_text(wifi_app->sw.label, "关");
        lv_label_set_text(options_screen->label.wifi, "关");
        wifi_info_set_status(false);
        lv_obj_clear_flag(wifi_app->text1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->text2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->text3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->saved.contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_app->add.contanier, LV_OBJ_FLAG_HIDDEN);
        wifi_destroy();
        lv_label_set_text_fmt(wifi_app->saved.label, "%s", wifi_info_get_ssid());
    }
}

static void saved_event_cb(lv_event_t *e)
{
    const lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_LONG_PRESSED) {
        static char * btns[] ={"忘记", "", ""};
        if (wifi_get_connected_status(0) == ESP_OK) {
            btns[1] = "断开";
        } else {
            btns[1] = "连接";
        }
        lv_obj_t * mbox = lv_msgbox_create(NULL, "已保存的网络", (const char*)wifi_info_get_ssid(), (const char**)btns, true);
        lv_obj_set_style_text_font(mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(mbox, saved_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_align(mbox, LV_ALIGN_CENTER, 0, -50);
        lv_obj_set_size(mbox, 176, 130);
        lv_obj_t * btnm = lv_msgbox_get_btns(mbox);
        lv_obj_set_style_bg_color(btnm, lv_color_hex(0xB1B2FF), LV_PART_ITEMS);
        lv_obj_set_style_bg_color(lv_msgbox_get_close_btn(mbox), lv_color_hex(0xB1B2FF), 0);
    }
}

static void saved_msgbox_event_cb(lv_event_t *e)
{
    lv_obj_t *mbox = lv_event_get_current_target(e);
    const lv_obj_t *btnm = lv_msgbox_get_btns(mbox);
    const uint32_t index = lv_btnmatrix_get_selected_btn(btnm);
    lv_msgbox_close(mbox);

    if (index == 1 && wifi_get_connected_status(0) == ESP_OK) {
        wifi_destroy();
        lv_label_set_text_fmt(wifi_app->saved.label, "%s", wifi_info_get_ssid());
    } else if (index == 1) {
        wifi_start();
        if (wifi_get_connected_status(portMAX_DELAY) == ESP_OK) {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "连接成功", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_fmt(wifi_app->saved.label, "%s（已连接）", wifi_info_get_ssid());
        } else {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "连接失败", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            wifi_destroy();
        }
    } else {
        wifi_destroy();
        wifi_info_set_ssid((uint8_t*)"");
        wifi_info_set_pwd((uint8_t*)"");
        wifi_info_set_authmode(WIFI_AUTH_OPEN);
        wifi_info_save_to_nvs();
    }
}

static void add_event_cb(lv_event_t *e)
{
    const lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_LONG_PRESSED) {
        static const char * btns[] ={""};
        lv_obj_t * mbox = lv_msgbox_create(NULL, "添加网络", "", btns, true);
        lv_obj_set_style_text_font(mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(mbox);
        lv_obj_set_size(mbox, 200, 200);

        lv_obj_t * name_label = lv_label_create(mbox);
        lv_obj_set_style_text_font(name_label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(name_label, "网络名称：");
        lv_obj_align(name_label, LV_ALIGN_TOP_RIGHT, 0, 0);

        /*Create the one-line mode text area*/
        lv_obj_t * ssid_ta = lv_textarea_create(mbox);
        lv_textarea_set_one_line(ssid_ta, true);
        lv_textarea_set_password_mode(ssid_ta, false);
        lv_textarea_set_placeholder_text(ssid_ta, "网络名称：");
        lv_obj_set_width(ssid_ta, lv_pct(90));
        lv_obj_align_to(ssid_ta, name_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -2);

        lv_obj_t * safe_label = lv_label_create(mbox);
        lv_obj_set_style_text_font(safe_label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(safe_label, "安全性：");
        lv_obj_align(safe_label, LV_ALIGN_TOP_RIGHT, 0, 9);

        /*Create a normal drop down list*/
        lv_obj_t * dd = lv_dropdown_create(mbox);
        lv_obj_set_width(dd, lv_pct(90));
        lv_obj_set_style_text_font(dd, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_dropdown_set_options(dd, "Open\n"
                                    "WEP\n"
                                    "WPA/WPA2-PSK\n"
                                    "WPA/WPA2-EAP\n"
                                    "WPA3-PSK");
        lv_obj_align_to(dd, safe_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
        lv_obj_add_event_cb(dd, add_msgbox_dropdown_event_cb, LV_EVENT_ALL, NULL);

        lv_obj_t * pwd_label = lv_label_create(mbox);
        lv_obj_set_style_text_font(pwd_label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(pwd_label, "密码：");
        lv_obj_align(pwd_label, LV_ALIGN_TOP_RIGHT, 0, 0);
        lv_obj_add_flag(pwd_label, LV_OBJ_FLAG_HIDDEN);

        lv_obj_t * pwd_ta = lv_textarea_create(mbox);
        lv_textarea_set_text(pwd_ta, "");
        lv_textarea_set_password_mode(pwd_ta, true);
        lv_textarea_set_one_line(pwd_ta, true);
        lv_textarea_set_placeholder_text(pwd_ta, "密码：");
        lv_obj_set_width(pwd_ta, lv_pct(90));
        lv_obj_align_to(pwd_ta, pwd_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        lv_obj_add_flag(pwd_ta, LV_OBJ_FLAG_HIDDEN);

        /*Create a keyboard*/
        lv_obj_t * kb = lv_keyboard_create(lv_layer_top());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(ssid_ta, add_msgbox_ta_event_cb, LV_EVENT_ALL, kb);
        lv_obj_add_event_cb(pwd_ta, add_msgbox_ta_event_cb, LV_EVENT_ALL, kb);
        lv_obj_add_event_cb(mbox, add_msgbox_event_cb, LV_EVENT_DELETE, kb);

        lv_obj_t *btn_save = lv_btn_create(mbox);
        lv_obj_add_event_cb(btn_save, add_msgbox_btn_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn_save, LV_ALIGN_TOP_MID, -40, 15);

        lv_obj_t * label = lv_label_create(btn_save);
        lv_obj_set_style_text_font(label, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(label, "保存");
        lv_obj_set_style_text_color(label, lv_color_black(), 0);
        lv_obj_center(label);

        lv_obj_t *btn_link = lv_btn_create(mbox);
        lv_obj_add_event_cb(btn_link, add_msgbox_btn_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn_link, LV_ALIGN_TOP_MID, 40, 15);
        lv_obj_set_height(btn_link, LV_SIZE_CONTENT);

        label = lv_label_create(btn_link);
        lv_label_set_text(label, "连接");
        lv_obj_set_style_text_color(label, lv_color_black(), 0);
        lv_obj_center(label);

        lv_obj_set_style_bg_color(btn_save, lv_color_hex(0xB1B2FF), 0);
        lv_obj_set_style_bg_color(btn_link, lv_color_hex(0xB1B2FF), 0);
        lv_obj_set_style_bg_color(lv_msgbox_get_close_btn(mbox), lv_color_hex(0xB1B2FF), 0);
    }
}

static void add_msgbox_ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);
    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}

static void add_msgbox_dropdown_event_cb(lv_event_t * e)
{
    const lv_event_code_t code = lv_event_get_code(e);
    const lv_obj_t *target = lv_event_get_target(e);
    const lv_obj_t *mbox = lv_obj_get_parent(target);
    lv_obj_t *pwd_label = lv_obj_get_child(mbox, 8);
    lv_obj_t *pwd_ta = lv_obj_get_child(mbox, 9);
    if(code == LV_EVENT_VALUE_CHANGED) {
        const uint16_t index = lv_dropdown_get_selected(target);
        if (index) {
            lv_obj_clear_flag(pwd_label, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(pwd_ta, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(pwd_label, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(pwd_ta, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void add_msgbox_btn_event_cb(lv_event_t *e)
{
    const lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *mbox = lv_obj_get_parent(target);
    const lv_obj_t *ssid_ta = lv_obj_get_child(mbox, 5);
    const lv_obj_t *dd = lv_obj_get_child(mbox, 7);
    const lv_obj_t *pwd_ta = lv_obj_get_child(mbox, 9);
    const lv_obj_t *btn_link = lv_obj_get_child(mbox, 11);

    const uint8_t *ssid = (uint8_t *)lv_textarea_get_text(ssid_ta);
    const uint8_t *password = (uint8_t *)lv_textarea_get_text(pwd_ta);
    wifi_auth_mode_t authmode = WIFI_AUTH_OPEN;

    switch (lv_dropdown_get_selected(dd)) {
        case 0: authmode=WIFI_AUTH_OPEN; break;
        case 1: authmode=WIFI_AUTH_WEP; break;
        case 2: authmode=WIFI_AUTH_WPA_WPA2_PSK; break;
        case 3: authmode=WIFI_AUTH_ENTERPRISE; break;
        case 4: authmode=WIFI_AUTH_WPA3_PSK; break;
        default: break;
    }

    if (authmode==WIFI_AUTH_OPEN) {
        wifi_info_set_ssid(ssid);
        wifi_info_set_pwd((uint8_t*)"");
        wifi_info_set_authmode(WIFI_AUTH_OPEN);
    } else {
        wifi_info_set_ssid(ssid);
        wifi_info_set_pwd(password);
        wifi_info_set_authmode(authmode);
    }

    wifi_info_save_to_nvs();
    if (strlen((char *)wifi_info_get_ssid()) == 0) {
        lv_label_set_text(wifi_app->saved.label, "无");
    } else {
        lv_label_set_text_fmt(wifi_app->saved.label, "%s", wifi_info_get_ssid());
    }

    lv_msgbox_close(mbox);
    wifi_destroy();

    if (target == btn_link) {
        wifi_start();
        if (wifi_get_connected_status(portMAX_DELAY) == ESP_OK) {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "连接成功", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_fmt(wifi_app->saved.label, "%s（已连接）", wifi_info_get_ssid());
        } else {
            lv_obj_t * _mbox = lv_msgbox_create(NULL, NULL, "连接失败", NULL, true);
            lv_obj_set_style_text_font(_mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            wifi_destroy();
        }
    }
}

static void add_msgbox_event_cb(lv_event_t *e)
{
    lv_obj_t * kb = lv_event_get_user_data(e);
    lv_obj_del_async(kb);
}

static void ui_event_return(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    options_screen_t *options_screen = lv_event_get_user_data(e);

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
        wifi_info_save_status_to_nvs();
    }
}