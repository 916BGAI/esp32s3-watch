#include "../ui.h"
#include "ui_helpers.h"

lv_obj_t *ui_main_screen;
static lv_obj_t *ui_image1;
static lv_obj_t *ui_button1;
static lv_obj_t *ui_label1;

static void ui_event_main_screen(lv_event_t *e);
static void ui_event_button1(lv_event_t *e);
static void led_event_callback(lv_event_t *e);

void ui_main_screen_init(void)
{
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(ui_main_screen, LV_FONT_DEFAULT,
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
    lv_obj_set_style_text_font(ui_button1, LV_FONT_DEFAULT,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_label1 = lv_label_create(ui_button1);
    lv_obj_set_width(ui_label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label1, "LED");
    lv_obj_set_style_text_font(ui_label1, LV_FONT_DEFAULT,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_button1, ui_event_button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_main_screen, ui_event_main_screen, LV_EVENT_ALL,
                        NULL);
}

static void ui_event_main_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_menu_screen_init);
    }
}

static void ui_event_button1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED) {
        led_event_callback(e);
    }
}

static void led_event_callback(lv_event_t *e)
{
    // Your code here
}