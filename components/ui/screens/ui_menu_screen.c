#include "../ui.h"
#include "ui_clock_screen.h"
#include "ui_options_screen.h"
#include "ui_menu_screen.h"

static menu_screen_t *menu_screen;

void app_reg(app_info_t *app_info);
static void app_button_event_cb(lv_event_t *e);

void ui_menu_screen_init(void)
{
    menu_screen = (menu_screen_t *)malloc(sizeof(menu_screen_t));
    menu_screen->screen = lv_obj_create(NULL);
    lv_obj_clear_flag(menu_screen->screen, LV_OBJ_FLAG_SCROLLABLE);

    menu_screen->tv = lv_tileview_create(menu_screen->screen);
    lv_obj_set_style_bg_opa(menu_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(menu_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_font(menu_screen->tv, &ui_font_sarasa18, LV_PART_MAIN | LV_STATE_DEFAULT);

    static app_info_t app1 = {
        .id = 1,
        .name = "设置",
        .img_src = &ui_img_options,
    };
    app_reg(&app1);

    static app_info_t app2 = {
        .id = 2,
        .name = "时钟",
        .img_src = &ui_img_clock,
    };
    app_reg(&app2);

    static app_info_t app3 = {
        .id = 3,
        .name = "设置",
        .img_src = &ui_img_weather,
    };
    app_reg(&app3);

    static app_info_t app4 = {
        .id = 4,
        .name = "设置",
        .img_src = &ui_img_more,
    };
    app_reg(&app4);

    lv_obj_add_event_cb(menu_screen->screen, ui_event_menu_screen, LV_EVENT_ALL, menu_screen);
}

void ui_event_clock_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
    }
}

void ui_event_options_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    options_screen_t *options_screen = (options_screen_t *)e->user_data;

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT &&
        options_screen->lock == false) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
        free(options_screen);
    }
}

void app_reg(app_info_t *app_info)
{
    static uint8_t tile_index = 0;
    if (tile_index > 3) {
        tile_index = 0;
    }
    menu_screen->tile = lv_tileview_add_tile(menu_screen->tv, tile_index++, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t *btn = lv_btn_create(menu_screen->tile);
    lv_obj_remove_style_all(btn);
    lv_obj_set_style_bg_opa(btn, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_size(btn, 90, 90);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn, app_button_event_cb, LV_EVENT_ALL, &app_info->id);
    lv_obj_t *img = lv_img_create(btn);
    lv_img_set_src(img, app_info->img_src);
    lv_obj_center(img);

    lv_obj_t *label = lv_label_create(menu_screen->tile);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 70);
    lv_label_set_text(label, app_info->name);
}

extern clock_screen_t clock_screen;

static void app_button_event_cb(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    const uint32_t id = *((uint32_t *)e->user_data);

    if (event_code == LV_EVENT_CLICKED) {
        switch (id) {
        case 1:
            ui_options_screen_init();
            break;
        case 2:
            lv_scr_load_anim(clock_screen.screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
        }

        free(menu_screen);
    }
}