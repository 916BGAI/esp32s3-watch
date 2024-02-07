#include "../ui.h"

static lv_obj_t *ui_options_screen;
static lv_obj_t *top_contanier;
static lv_obj_t *top_label;
static lv_obj_t *return_button;
static lv_obj_t *save_button;
static lv_obj_t * options_list;
static lv_style_t style;

static void brightness_event_callback(lv_event_t *e)
{

}

void ui_options_screen_init(void)
{
    ui_options_screen = lv_obj_create(NULL);
    lv_obj_set_style_text_font(ui_options_screen, &ui_font_sarasa16,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    top_contanier = lv_obj_create(ui_options_screen);
    lv_obj_clear_flag(top_contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(top_contanier, 240, 30);
    lv_obj_align(top_contanier, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(top_contanier, lv_color_hex(0xEFF0F1), 0);

    top_label = lv_label_create(top_contanier);
    lv_obj_set_style_text_font(top_label, &ui_font_sarasa18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(top_label, "设 置");
    lv_obj_align(top_label, LV_ALIGN_CENTER, 0, 0);

    return_button = lv_btn_create(top_contanier);
    lv_obj_set_size(return_button, 35, 20);
    lv_obj_align(return_button, LV_ALIGN_TOP_LEFT, -5, -7);
    lv_obj_add_flag(return_button, LV_OBJ_FLAG_HIDDEN);

    save_button = lv_btn_create(top_contanier);
    lv_obj_set_size(save_button, 35, 20);
    lv_obj_align(save_button, LV_ALIGN_TOP_RIGHT, 5, -7);
    lv_obj_add_flag(save_button, LV_OBJ_FLAG_HIDDEN);

    options_list = lv_list_create(ui_options_screen);
    lv_obj_set_size(options_list, 240, 250);
    lv_obj_align(options_list, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t *btn;
    btn = lv_list_add_btn(options_list, NULL, "屏幕亮度");
    lv_obj_add_event_cb(btn, brightness_event_callback, LV_EVENT_CLICKED, NULL);

    lv_style_init(&style);
    lv_style_set_border_width(&style, 0);
    lv_obj_add_style(options_list, &style, 0);

    lv_disp_load_scr(ui_options_screen);
}