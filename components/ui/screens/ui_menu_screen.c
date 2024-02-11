#include "../ui.h"
#include "ui_helpers.h"

lv_obj_t *ui_menu_screen;
static lv_obj_t *container;
static lv_obj_t *running_application;

static void application_button_event_cb(lv_event_t *event);
static void ui_event_menu_screen(lv_event_t *e);

static void application_reg(application_info_t *application_info)
{
    lv_obj_t *btn = lv_btn_create(container);
    lv_obj_set_size(btn, 90, 90);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_FOCUSED,
                        application_info->name);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_CLICKED,
                        application_info);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_SIZE_CHANGED,
                        NULL);

    lv_obj_remove_style_all(btn);
    lv_obj_set_style_bg_opa(btn, LV_OPA_0, LV_PART_MAIN);

    if (application_info->img_src != NULL) {
        lv_obj_t *img = lv_img_create(btn);
        lv_img_set_src(img, application_info->img_src);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }
}

void ui_menu_screen_init(void)
{
    ui_menu_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui_menu_screen, lv_color_hex(0x8C40DE),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_menu_screen, 255,
                            LV_PART_MAIN | LV_STATE_DEFAULT);

    container = lv_obj_create(ui_menu_screen);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_style_bg_opa(container, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(container, 30, LV_PART_MAIN); //图标之间的间隙
    lv_obj_center(container);

    static application_info_t app1 = {
        .name = "APP OPTIONS",
        .img_src = &ui_img_options_png,
        .entry_point = (lv_obj_t * (*)(void *))ui_options_screen_init,
        .release_resource_cb = NULL,
    };
    application_reg(&app1);

    static application_info_t app2 = {
        .name = "FILE_MANAGER",
        .entry_point = (lv_obj_t * (*)(void *))ui_options_screen_init,
        .img_src = &ui_img_more_png,
        .release_resource_cb = NULL,
    };
    application_reg(&app2);

    static application_info_t app3 = {
        .name = "APP_ABOUT",
        .img_src = &ui_img_weather_png,
        .entry_point = (lv_obj_t * (*)(void *))ui_options_screen_init,
        .release_resource_cb = NULL,
    };
    application_reg(&app3);

    static application_info_t app4 = {
        .name = "APP_ABOUT",
        .img_src = &ui_img_clock_png,
        .entry_point = (lv_obj_t * (*)(void *))ui_options_screen_init,
        .release_resource_cb = NULL,
    };
    application_reg(&app4);

    uint32_t mid_btn_index = (lv_obj_get_child_cnt(container) - 1) / 2;
    for (uint32_t i = 0; i < mid_btn_index; i++) {
        lv_obj_move_to_index(lv_obj_get_child(container, -1), 0);
    }
    /*当按钮数为偶数时，确保按钮居中*/
    lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index),
                          LV_ANIM_OFF);

    lv_obj_add_event_cb(ui_menu_screen, ui_event_menu_screen, LV_EVENT_ALL,
                        NULL);
}

static void application_button_event_cb(lv_event_t *event)
{
    lv_obj_t *current_btn = lv_event_get_current_target(event);
    if (event->code == LV_EVENT_FOCUSED) {
        uint32_t current_btn_index = lv_obj_get_index(current_btn);
        uint32_t mid_btn_index = (lv_obj_get_child_cnt(container) - 1) / 2;

        if (current_btn_index > mid_btn_index) {
            lv_obj_scroll_to_view(
                lv_obj_get_child(container, mid_btn_index - 1), LV_ANIM_OFF);
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index),
                                  LV_ANIM_ON);
            lv_obj_move_to_index(lv_obj_get_child(container, 0), -1);
        } else if (current_btn_index < mid_btn_index) {
            lv_obj_scroll_to_view(
                lv_obj_get_child(container, mid_btn_index + 1), LV_ANIM_OFF);
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index),
                                  LV_ANIM_ON);
            lv_obj_move_to_index(lv_obj_get_child(container, -1), 0);
        }
        for (uint8_t i = 0; i < 3; i++) {
            lv_obj_set_size(lv_obj_get_child(container, mid_btn_index - i),
                            90 - i * 10, 90 - i * 10);
            lv_obj_set_size(lv_obj_get_child(container, mid_btn_index + i),
                            90 - i * 10, 90 - i * 10);
            lv_obj_set_style_bg_opa(lv_obj_get_child(container,
                                                     mid_btn_index - i),
                                    255 - 50 * i, LV_PART_MAIN);
            lv_obj_set_style_bg_opa(lv_obj_get_child(container,
                                                     mid_btn_index + i),
                                    255 - 50 * i, LV_PART_MAIN);
        }
    } else if (event->code == LV_EVENT_SIZE_CHANGED) {
        /*缩放图标*/
        lv_obj_t *img = lv_obj_get_child(current_btn, 0);

        if (lv_obj_is_valid(img)) {
            lv_img_set_zoom(img, (uint16_t)(lv_obj_get_width(current_btn) *
                                            0.7 / 64 * LV_IMG_ZOOM_NONE));
        }
    } else if (event->code == LV_EVENT_CLICKED) {
        /*获取正准备打开APP的信息*/
        application_info_t *application_info = lv_event_get_user_data(event);
        lv_obj_t *(*app_entry_point)(lv_obj_t *parent) =
            (lv_obj_t * (*)(lv_obj_t *))application_info->entry_point;

        /*创建一个基本对象，用于承载APP的控件*/
        lv_obj_t *base_obj = lv_obj_create(lv_scr_act());
        lv_obj_set_size(base_obj, LV_PCT(100), LV_PCT(100));
        lv_obj_set_scrollbar_mode(base_obj, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_pad_all(base_obj, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_top(base_obj, 30, LV_PART_MAIN);
        lv_obj_set_style_pad_row(base_obj, 0, LV_PART_MAIN);

        /*打开APP并将基本对象赋值给当前正在运行的程序，返回按钮触发时将删除该对象*/
        running_application = app_entry_point(base_obj);
        /*添加一个删除事件到基本对象，当APP退出时调用该回调函数释放定时器、数据结构等资源*/
        lv_obj_add_event_cb(base_obj, application_info->release_resource_cb,
                            LV_EVENT_DELETE, NULL);
        running_application = base_obj;
    }
}

static void ui_event_menu_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0,
                          &ui_main_screen_init);
    }
}