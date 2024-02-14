#include "../ui.h"
#include "ui_brightness_app.h"
#include "ui_options_screen.h"

static void ui_event_options_screen(lv_event_t *e);

void ui_options_screen_init(void)
{
    options_screen_t *options_screen = (options_screen_t *)malloc(sizeof(options_screen_t));
    options_screen->lock = false;

    options_screen->screen = lv_obj_create(NULL);

    options_screen->top_contanier = lv_obj_create(options_screen->screen);
    lv_obj_set_style_text_font(options_screen->screen, &ui_font_sarasa16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(options_screen->top_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(options_screen->top_contanier, 240, 40);
    lv_obj_align(options_screen->top_contanier, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(options_screen->top_contanier, lv_color_hex(0xEFF0F1), 0);

    options_screen->top_label = lv_label_create(options_screen->top_contanier);
    lv_obj_set_style_text_font(options_screen->top_label, &ui_font_sarasa18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_screen->top_label, "设 置");
    lv_obj_align(options_screen->top_label, LV_ALIGN_CENTER, 0, 0);

    options_screen->return_button = lv_btn_create(options_screen->top_contanier);
    lv_obj_t *return_button_label = lv_label_create(options_screen->return_button);
    lv_obj_set_size(options_screen->return_button, 39, 27);
    lv_obj_align(options_screen->return_button, LV_ALIGN_LEFT_MID, -25, 0);
    lv_label_set_text(return_button_label, "返回");
    lv_obj_center(return_button_label);
    lv_obj_add_event_cb(options_screen->return_button, return_save_button_event_callback, LV_EVENT_CLICKED,
                        (void *)options_screen);
    lv_obj_add_flag(options_screen->return_button, LV_OBJ_FLAG_HIDDEN);

    options_screen->save_button = lv_btn_create(options_screen->top_contanier);
    lv_obj_t *save_button_label = lv_label_create(options_screen->save_button);
    lv_obj_set_size(options_screen->save_button, 39, 27);
    lv_obj_align(options_screen->save_button, LV_ALIGN_RIGHT_MID, 25, 0);
    lv_label_set_text(save_button_label, "保存");
    lv_obj_center(save_button_label);
    lv_obj_add_event_cb(options_screen->save_button, return_save_button_event_callback, LV_EVENT_CLICKED,
                        (void *)options_screen);
    lv_obj_add_flag(options_screen->save_button, LV_OBJ_FLAG_HIDDEN);

    options_screen->list = lv_list_create(options_screen->screen);
    lv_obj_set_size(options_screen->list, 240, 240);
    lv_obj_align(options_screen->list, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_BRIGHTNESS, "屏幕亮度");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, (void *)options_screen);
    btn = lv_list_add_btn(options_screen->list, UI_SYMBOL_WIFI, "Wi-Fi");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, (void *)options_screen);

    lv_style_init(&options_screen->style);
    lv_style_set_border_width(&options_screen->style, 0);
    lv_obj_add_style(options_screen->list, &options_screen->style, 0);

    lv_obj_add_event_cb(options_screen->screen, ui_event_options_screen, LV_EVENT_ALL, (void *)options_screen);
    lv_disp_load_scr(options_screen->screen);
}

static void ui_event_options_screen(lv_event_t *e)
{
    options_screen_t *options_screen = (options_screen_t *)e->user_data;

    const lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->lock == false) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_scr_load_anim(ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        free(options_screen);
    }
}