#include "../ui.h"
#include "ui_brightness_app.h"

lv_obj_t *ui_options_screen;
lv_obj_t * options_list;
lv_obj_t *options_top_contanier;
lv_obj_t *options_top_label;
lv_obj_t *options_return_button;
lv_obj_t *options_save_button;
lv_style_t options_style;

void ui_options_screen_init(void)
{
    ui_options_screen = lv_obj_create(NULL);

    options_top_contanier = lv_obj_create(ui_options_screen);
    lv_obj_clear_flag(options_top_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(options_top_contanier, 240, 30);
    lv_obj_align(options_top_contanier, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(options_top_contanier, lv_color_hex(0xEFF0F1), 0);

    options_top_label = lv_label_create(options_top_contanier);
    lv_obj_set_style_text_font(options_top_label, &ui_font_sarasa18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(options_top_label, "设 置");
    lv_obj_align(options_top_label, LV_ALIGN_CENTER, 0, 0);

    options_return_button = lv_btn_create(options_top_contanier);
    lv_obj_t *return_button_label = lv_label_create(options_return_button);
    lv_obj_set_size(options_return_button, 35, 20);
    lv_obj_align(options_return_button, LV_ALIGN_LEFT_MID, -5, 0);
    lv_label_set_text(return_button_label, "return");
    lv_obj_center(return_button_label);
    lv_obj_add_event_cb(options_return_button, return_save_button_event_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(options_return_button, LV_OBJ_FLAG_HIDDEN);

    options_save_button = lv_btn_create(options_top_contanier);
    lv_obj_t *save_button_label = lv_label_create(options_save_button);
    lv_obj_set_size(options_save_button, 35, 20);
    lv_obj_align(options_save_button, LV_ALIGN_RIGHT_MID, 5, 0);
    lv_label_set_text(save_button_label, "save");
    lv_obj_center(save_button_label);
    lv_obj_add_event_cb(options_save_button, return_save_button_event_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(options_save_button, LV_OBJ_FLAG_HIDDEN);

    options_list = lv_list_create(ui_options_screen);
    lv_obj_set_style_text_font(options_list, &ui_font_sarasa16,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(options_list, 240, 250);
    lv_obj_align(options_list, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t * btn;
    btn = lv_list_add_btn(options_list, NULL, "屏幕亮度");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, NULL);

    lv_style_init(&options_style);
    lv_style_set_border_width(&options_style,0);
    lv_obj_add_style(options_list, &options_style, 0);

    lv_disp_load_scr(ui_options_screen);
}