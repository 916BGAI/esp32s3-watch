#include "../ui.h"
#include "ui_clock_screen.h"
#include "ui_menu_screen.h"
#include "ui_timer_screen.h"
#include "wifi.h"

menu_screen_t *menu_screen;

static void app_reg(app_info_t *app_info);
static void ui_event_menu_screen(lv_event_t *e);
static void app_button_event_cb(lv_event_t *e);

void ui_menu_screen_init(void)
{
    menu_screen = malloc(sizeof(menu_screen_t));
    menu_screen->screen = lv_obj_create(NULL);
    lv_obj_clear_flag(menu_screen->screen, LV_OBJ_FLAG_SCROLLABLE);

    menu_screen->tv = lv_tileview_create(menu_screen->screen);
    lv_obj_set_style_bg_opa(menu_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(menu_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_font(menu_screen->tv, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    static app_info_t app1 = {
        .id = 1,
        .name = "设置",
        .img_src = "A:images/menu/options.bin",
    };
    app_reg(&app1);

    static app_info_t app2 = {
        .id = 2,
        .name = "时钟",
        .img_src = "A:images/menu/clock.bin",
    };
    app_reg(&app2);

    static app_info_t app3 = {
        .id = 3,
        .name = "天气",
        .img_src = "A:images/menu/weather.bin",
    };
    app_reg(&app3);

    static app_info_t app4 = {
        .id = 4,
        .name = "计时",
        .img_src = "A:images/menu/timer.bin",
    };
    app_reg(&app4);

    static app_info_t app5 = {
        .id = 5,
        .name = "定位",
        .img_src = "A:images/menu/position.bin",
    };
    app_reg(&app5);

    static app_info_t app6 = {
        .id = 6,
        .name = "设置",
        .img_src = "A:images/menu/more.bin",
    };
    app_reg(&app6);

    lv_obj_add_event_cb(menu_screen->screen, ui_event_menu_screen, LV_EVENT_ALL, NULL);
}

void app_reg(app_info_t *app_info)
{
    static uint8_t tile_index = 0;
    if (tile_index > 5) {
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
void ui_event_menu_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_scr_load_anim(clock_screen.screen, LV_SCR_LOAD_ANIM_OUT_TOP, 300, 0, true);
        free(menu_screen);
        menu_screen = NULL;
    }
}

extern clock_screen_t clock_screen;
extern timer_screen_t *timer_screen;
static void app_button_event_cb(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);
    const uint32_t id = *((uint32_t *)e->user_data);

    if (event_code == LV_EVENT_CLICKED) {
        switch (id) {
        case 1:
            ui_options_screen_init();
            free(menu_screen);
            menu_screen = NULL;
            break;
        case 2:
            lv_scr_load_anim(clock_screen.screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
            free(menu_screen);
            menu_screen = NULL;
            break;
        case 3:
            if (wifi_get_connected_status(0) == ESP_OK) {
                ui_weather_screen_init();
                free(menu_screen);
                menu_screen = NULL;
            } else {
                lv_obj_t * mbox = lv_msgbox_create(NULL, "天气", "若要查看天气，\n请连接Wi-Fi", NULL, true);
                lv_obj_set_style_text_font(mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_align(mbox, LV_ALIGN_CENTER, 0, 0);
                lv_obj_set_size(mbox, 176, 120);
            }
            break;
        case 4:
            if(get_timer_config().status == STOP) {
                ui_timer_screen_init();
                lv_scr_load_anim(timer_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
            } else {
                lv_scr_load_anim(timer_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
            }
            free(menu_screen);
            menu_screen = NULL;
            break;
        case 5:
            ui_gps_screen_init();
            free(menu_screen);
            menu_screen = NULL;
            break;
        case 6:
            break;
        default:
            break;
        }
    }
}