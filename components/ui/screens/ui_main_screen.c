#include "../ui.h"

void ui_main_screen_init(void)
{
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(ui_main_screen, &lv_font_montserrat_12,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_image1 = lv_img_create(ui_main_screen);
    lv_img_set_src(ui_image1, &ui_img_image_png);
    lv_obj_set_width(ui_image1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_image1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image1, LV_OBJ_FLAG_SCROLLABLE);

    ui_button1 = lv_btn_create(ui_main_screen);
    lv_obj_set_width(ui_button1, 50);
    lv_obj_set_height(ui_button1, 25);
    lv_obj_set_x(ui_button1, 0);
    lv_obj_set_y(ui_button1, 90);
    lv_obj_set_align(ui_button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_button1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(ui_button1, &lv_font_montserrat_12,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_label1 = lv_label_create(ui_button1);
    lv_obj_set_width(ui_label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label1, "LED");
    lv_obj_set_style_text_font(ui_label1, &lv_font_montserrat_12,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_button1, ui_event_button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_main_screen, ui_event_main_screen, LV_EVENT_ALL,
                        NULL);
}
