#include "../ui.h"

lv_obj_t* container;
lv_obj_t* running_application;

static void application_button_event_cb(lv_event_t* event);

/*硬件信息*/
static const char* MCU_Model = "ESP32-S3";
static lv_obj_t* cpu_model_label;
static lv_obj_t* cpu_temperature_label;
static lv_obj_t* internal_heap_usage_label;
static lv_obj_t* external_heap_usage_label;
static lv_obj_t* sdcard_cap_label;

static void app_about_update_data(lv_timer_t timer)
{
    lv_label_set_text_fmt(lv_obj_get_child(cpu_temperature_label, 1), "%d℃", (int)lv_rand(0, 100));
    lv_label_set_text_fmt(lv_obj_get_child(internal_heap_usage_label, 1), "25%%\n2.1MB/8MB");
    lv_label_set_text_fmt(lv_obj_get_child(external_heap_usage_label, 1), "25%%\n2.1MB/8MB");
    lv_label_set_text_fmt(lv_obj_get_child(sdcard_cap_label, 1), "%.1f/%.1f", 3.1, 59.2);
}

static lv_timer_t* app_about_update_data_timer;

static lv_obj_t* about_list_add_item(lv_obj_t* parent, const char* name, const char* value)
{
    lv_obj_t* about_item = lv_btn_create(parent);
    lv_obj_remove_style_all(about_item);
    lv_obj_set_style_border_side(about_item, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(about_item, 1, LV_PART_MAIN);
    lv_obj_set_style_bg_color(about_item, lv_color_black(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(about_item, LV_OPA_20, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(about_item, LV_OPA_50, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_pad_left(about_item, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_right(about_item, 10, LV_PART_MAIN);
    lv_obj_set_size(about_item, LV_HOR_RES, 40);

    lv_obj_t* item_name = lv_label_create(about_item);
    lv_label_set_text(item_name, name);
    lv_obj_set_style_text_font(item_name, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_align(item_name, LV_ALIGN_LEFT_MID);

    lv_obj_t* item_value = lv_label_create(about_item);
    lv_label_set_text(item_value, value);
    lv_label_set_long_mode(item_value, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(item_value, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(item_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
    lv_obj_set_align(item_value, LV_ALIGN_RIGHT_MID);

    return about_item;
}

void app_about(void* user_date)
{
    lv_obj_t* app_about_obj = lv_obj_create(lv_scr_act());
    lv_obj_set_scroll_dir(app_about_obj, LV_DIR_VER);
    lv_obj_set_flex_flow(app_about_obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(app_about_obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(app_about_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(app_about_obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_top(app_about_obj, 30, LV_PART_MAIN);
    lv_obj_set_style_pad_row(app_about_obj, 0, LV_PART_MAIN);

    cpu_model_label = about_list_add_item(app_about_obj, "CPU型号", MCU_Model);
    cpu_temperature_label = about_list_add_item(app_about_obj, "CPU温度", "");
    internal_heap_usage_label = about_list_add_item(app_about_obj, "SRAM剩余", "");
    external_heap_usage_label = about_list_add_item(app_about_obj, "PSRAM剩余", "");
    sdcard_cap_label = about_list_add_item(app_about_obj, "SD卡剩余", "");

    app_about_update_data_timer = lv_timer_create(app_about_update_data, 1000, NULL);

}

static void application_reg(application_info_t* application_info)
{
    lv_obj_t* btn = lv_btn_create(container);
    lv_obj_set_size(btn, 90, 90);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_FOCUSED, application_info->name);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_CLICKED, application_info);
    lv_obj_add_event_cb(btn, application_button_event_cb, LV_EVENT_SIZE_CHANGED, NULL);

    lv_obj_remove_style_all(btn);
    lv_obj_set_style_bg_opa(btn, LV_OPA_0, LV_PART_MAIN);

    if (application_info->img_src != NULL)
    {
        lv_obj_t* img = lv_img_create(btn);
        lv_img_set_src(img, application_info->img_src);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }
}

void ui_menuScreen_screen_init(void)
{
    ui_menuScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui_menuScreen, lv_color_hex(0x8C40DE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_menuScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    container = lv_obj_create(ui_menuScreen);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_style_bg_opa(container, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(container, 30, LV_PART_MAIN);   //图标之间的间隙
    lv_obj_center(container);

    static application_info_t app1 =
    {
        .name = "APP OPTIONS",
        .img_src = &ui_img_options_png,
        .entry_point = app_about,
        .release_resource_cb = NULL
    };
    application_reg(&app1);

    static application_info_t app2 =
    {
        .name = "FILE_MANAGER",
        .entry_point = app_about,
        .img_src = &ui_img_more_png,
        .release_resource_cb = NULL
    };
    application_reg(&app2);

    static application_info_t app3 =
    {
        .name = "APP_ABOUT",
        .img_src = &ui_img_weather_png,
        .entry_point = app_about,
        .release_resource_cb = NULL
    };
    application_reg(&app3);

    static application_info_t app4 =
    {
        .name = "APP_ABOUT",
        .img_src = &ui_img_clock_png,
        .entry_point = app_about,
        .release_resource_cb = NULL
    };
    application_reg(&app4);

    uint32_t mid_btn_index = (lv_obj_get_child_cnt(container) - 1) / 2;
    for (uint32_t i = 0; i < mid_btn_index; i++)
    {
        lv_obj_move_to_index(lv_obj_get_child(container, -1), 0);
    }
    /*当按钮数为偶数时，确保按钮居中*/
    lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index), LV_ANIM_OFF);

    lv_obj_add_event_cb(ui_menuScreen, ui_event_menuScreen, LV_EVENT_ALL, NULL);

}

static void application_button_event_cb(lv_event_t* event)
{
    lv_obj_t* current_btn = lv_event_get_current_target(event);
    if (event->code == LV_EVENT_FOCUSED)
    {
        uint32_t current_btn_index = lv_obj_get_index(current_btn);
        uint32_t mid_btn_index = (lv_obj_get_child_cnt(container) - 1) / 2;

        if (current_btn_index > mid_btn_index)
        {
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index - 1), LV_ANIM_OFF);
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index), LV_ANIM_ON);
            lv_obj_move_to_index(lv_obj_get_child(container, 0), -1);
        }
        else if (current_btn_index < mid_btn_index)
        {
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index + 1), LV_ANIM_OFF);
            lv_obj_scroll_to_view(lv_obj_get_child(container, mid_btn_index), LV_ANIM_ON);
            lv_obj_move_to_index(lv_obj_get_child(container, -1), 0);
        }
        for (uint8_t i = 0; i < 3; i++)
        {
            lv_obj_set_size(lv_obj_get_child(container, mid_btn_index - i), 90 - i * 10, 90 - i * 10);
            lv_obj_set_size(lv_obj_get_child(container, mid_btn_index + i), 90 - i * 10, 90 - i * 10);
            lv_obj_set_style_bg_opa(lv_obj_get_child(container, mid_btn_index - i), 255 - 50 * i, LV_PART_MAIN);
            lv_obj_set_style_bg_opa(lv_obj_get_child(container, mid_btn_index + i), 255 - 50 * i, LV_PART_MAIN);
        }
    }
    else if (event->code == LV_EVENT_SIZE_CHANGED)
    {
        /*缩放图标*/
        lv_obj_t* img = lv_obj_get_child(current_btn, 0);
        
        if (lv_obj_is_valid(img))
        {
            lv_img_set_zoom(img, (uint16_t)(lv_obj_get_width(current_btn) * 0.7 / 64 * LV_IMG_ZOOM_NONE));
        }
    }
    else if (event->code == LV_EVENT_CLICKED)
    {
        /*获取正准备打开APP的信息*/
        application_info_t* application_info = lv_event_get_user_data(event);
        lv_obj_t* (*app_entry_point)(lv_obj_t * parent) = application_info->entry_point;

        /*创建一个基本对象，用于承载APP的控件*/
        lv_obj_t* base_obj = lv_obj_create(lv_scr_act());
        lv_obj_set_size(base_obj, LV_PCT(100), LV_PCT(100));
        lv_obj_set_scrollbar_mode(base_obj, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_pad_all(base_obj, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_top(base_obj, 30, LV_PART_MAIN);
        lv_obj_set_style_pad_row(base_obj, 0, LV_PART_MAIN);

        /*打开APP并将基本对象赋值给当前正在运行的程序，返回按钮触发时将删除该对象*/
        running_application = app_entry_point(base_obj);
        /*添加一个删除事件到基本对象，当APP退出时调用该回调函数释放定时器、数据结构等资源*/
        lv_obj_add_event_cb(base_obj, application_info->release_resource_cb, LV_EVENT_DELETE, NULL);
        running_application = base_obj;
    }

}
