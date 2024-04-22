#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_timer_screen.h"

timer_screen_t *timer_screen;
static timer_config_t timer_config = {
    .status = false,
    .hour = 0,
    .min = 0,
    .sec = 0,
};

static void mask_event_cb(lv_event_t * e);
static void btn_event_cb(lv_event_t *e);
static void timer_cb(lv_timer_t *timer);
static void ui_event_timer_screen(lv_event_t *e);

void ui_timer_screen_init(void)
{
    if (timer_screen == NULL) {
        timer_screen = malloc(sizeof(timer_screen_t));
    }

    timer_screen->screen = lv_obj_create(NULL);

    lv_obj_t * label_text = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(label_text, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_text, "计时");
    lv_obj_align(label_text, LV_ALIGN_DEFAULT, 20, 20);

    timer_screen->roller.hour = lv_roller_create(timer_screen->screen);
    lv_obj_set_style_text_line_space(timer_screen->roller.hour, 3, LV_PART_MAIN);
    lv_obj_set_style_border_width(timer_screen->roller.hour, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.hour, lv_color_hex(0xF6F6F6), LV_PART_MAIN);
    lv_obj_set_style_text_font(timer_screen->roller.hour, SarasaMonoB_30, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.hour, lv_color_hex(0xF6F6F6), LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(timer_screen->roller.hour, LV_OPA_TRANSP, LV_PART_SELECTED);
    lv_obj_set_style_text_color(timer_screen->roller.hour, lv_color_black(), LV_PART_SELECTED);
    lv_obj_set_style_text_font(timer_screen->roller.hour, SarasaMonoB_30, LV_PART_SELECTED);
    const char* hour_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                               "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
    lv_roller_set_options(timer_screen->roller.hour, hour_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.hour, 3);
    lv_obj_align(timer_screen->roller.hour, LV_ALIGN_TOP_MID, -70, 76);
    lv_obj_set_size(timer_screen->roller.hour, 50, 109);

    lv_obj_t * label_hour = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(label_hour, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_hour, "时");
    lv_obj_align_to(label_hour, timer_screen->roller.hour, LV_ALIGN_OUT_RIGHT_MID, -6, 0);

    timer_screen->roller.min = lv_roller_create(timer_screen->screen);
    lv_obj_set_style_text_line_space(timer_screen->roller.min, 3, LV_PART_MAIN);
    lv_obj_set_style_border_width(timer_screen->roller.min, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.min, lv_color_hex(0xF6F6F6), LV_PART_MAIN);
    lv_obj_set_style_text_font(timer_screen->roller.min, SarasaMonoB_30, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.min, lv_color_hex(0xF6F6F6), LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(timer_screen->roller.min, LV_OPA_TRANSP, LV_PART_SELECTED);
    lv_obj_set_style_text_color(timer_screen->roller.min, lv_color_black(), LV_PART_SELECTED);
    lv_obj_set_style_text_font(timer_screen->roller.min, SarasaMonoB_30, LV_PART_SELECTED);
    const char* min_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                              "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                              "\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36"
                              "\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48"
                              "\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    lv_roller_set_options(timer_screen->roller.min, min_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.min, 3);
    lv_obj_align(timer_screen->roller.min, LV_ALIGN_TOP_MID, 0, 76);
    lv_obj_set_size(timer_screen->roller.min, 50, 109);

    lv_obj_t * label_min = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(label_min, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_min, "分");
    lv_obj_align_to(label_min, timer_screen->roller.min, LV_ALIGN_OUT_RIGHT_MID, -6, 0);

    timer_screen->roller.sec = lv_roller_create(timer_screen->screen);
    lv_obj_set_style_text_line_space(timer_screen->roller.sec, 3, LV_PART_MAIN);
    lv_obj_set_style_border_width(timer_screen->roller.sec, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.sec, lv_color_hex(0xF6F6F6), LV_PART_MAIN);
    lv_obj_set_style_text_font(timer_screen->roller.sec, SarasaMonoB_30, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timer_screen->roller.sec, lv_color_hex(0xF6F6F6), LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(timer_screen->roller.sec, LV_OPA_TRANSP, LV_PART_SELECTED);
    lv_obj_set_style_text_color(timer_screen->roller.sec, lv_color_black(), LV_PART_SELECTED);
    lv_obj_set_style_text_font(timer_screen->roller.sec, SarasaMonoB_30, LV_PART_SELECTED);
    const char* sec_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                              "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                              "\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36"
                              "\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48"
                              "\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    lv_roller_set_options(timer_screen->roller.sec, sec_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.sec, 3);
    lv_obj_align(timer_screen->roller.sec, LV_ALIGN_TOP_MID, 70, 76);
    lv_obj_set_size(timer_screen->roller.sec, 50, 109);

    lv_obj_add_event_cb(timer_screen->roller.hour, mask_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(timer_screen->roller.min, mask_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(timer_screen->roller.sec, mask_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label_sec = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(label_sec, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_sec, "秒");
    lv_obj_align_to(label_sec, timer_screen->roller.sec, LV_ALIGN_OUT_RIGHT_MID, -6, 0);

    timer_screen->btn.start = lv_btn_create(timer_screen->screen);
    lv_obj_set_style_bg_color(timer_screen->btn.start, lv_color_hex(0xD6EAF8), LV_PART_MAIN);
    lv_obj_set_style_radius(timer_screen->btn.start, LV_PCT(20), LV_PART_MAIN);
    lv_obj_add_event_cb(timer_screen->btn.start, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(timer_screen->btn.start, LV_ALIGN_BOTTOM_MID, 0, -33);
    lv_obj_set_size(timer_screen->btn.start, 70, 36);

    lv_obj_t *btn_img = lv_img_create(timer_screen->btn.start);
    lv_img_set_src(btn_img, "A:images/timer/start.bin");
    lv_obj_align(btn_img, LV_ALIGN_CENTER, 0, 0);

    timer_screen->btn.stop = lv_btn_create(timer_screen->screen);
    lv_obj_set_style_bg_color(timer_screen->btn.stop, lv_color_hex(0xD6EAF8), LV_PART_MAIN);
    lv_obj_set_style_radius(timer_screen->btn.stop, LV_PCT(100), LV_PART_MAIN);
    lv_obj_add_event_cb(timer_screen->btn.stop, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(timer_screen->btn.stop, LV_ALIGN_BOTTOM_MID, -50, -33);
    lv_obj_set_size(timer_screen->btn.stop, 40, 36);
    lv_obj_add_flag(timer_screen->btn.stop, LV_OBJ_FLAG_HIDDEN);

    btn_img = lv_img_create(timer_screen->btn.stop);
    lv_img_set_src(btn_img, "A:images/timer/stop.bin");
    lv_obj_align(btn_img, LV_ALIGN_CENTER, 0, 0);

    timer_screen->btn.pause = lv_btn_create(timer_screen->screen);
    lv_obj_set_style_bg_color(timer_screen->btn.pause, lv_color_hex(0xD6EAF8), LV_PART_MAIN);
    lv_obj_set_style_radius(timer_screen->btn.pause, LV_PCT(100), LV_PART_MAIN);
    lv_obj_add_event_cb(timer_screen->btn.pause, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(timer_screen->btn.pause, LV_ALIGN_BOTTOM_MID, 50, -33);
    lv_obj_set_size(timer_screen->btn.pause, 40, 36);
    lv_obj_add_flag(timer_screen->btn.pause, LV_OBJ_FLAG_HIDDEN);

    btn_img = lv_img_create(timer_screen->btn.pause);
    lv_img_set_src(btn_img, "A:images/timer/pause.bin");
    lv_obj_align(btn_img, LV_ALIGN_CENTER, 0, 0);

    timer_screen->label_time = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(timer_screen->label_time, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(timer_screen->label_time, "00:00:00");
    lv_obj_align_to(timer_screen->label_time, timer_screen->roller.min, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(timer_screen->label_time, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(timer_screen->screen, ui_event_timer_screen, LV_EVENT_ALL, NULL);
}

static void mask_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if(code == LV_EVENT_COVER_CHECK) {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);

    }
    else if(code == LV_EVENT_DRAW_MAIN_BEGIN) {
        /* add mask */
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        int32_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        int32_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t * fade_mask_top = malloc(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t * fade_mask_bottom = malloc(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);

    }
    else if(code == LV_EVENT_DRAW_POST_END) {
        lv_draw_mask_fade_param_t * fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t * fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        free(fade_mask_top);
        fade_mask_top = NULL;
        free(fade_mask_bottom);
        fade_mask_bottom = NULL;
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

static void btn_event_cb(lv_event_t *e)
{
    static lv_obj_t *label_time_text = NULL;
    const lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *btn_img = lv_obj_get_child(timer_screen->btn.pause, 0);
    lv_obj_t *label_hour = lv_obj_get_child(timer_screen->screen, 2);
    lv_obj_t *label_min = lv_obj_get_child(timer_screen->screen, 4);
    lv_obj_t *label_sec = lv_obj_get_child(timer_screen->screen, 6);

    if (target == timer_screen->btn.start && lv_roller_get_selected(timer_screen->roller.hour) +
        lv_roller_get_selected(timer_screen->roller.min) + lv_roller_get_selected(timer_screen->roller.sec) != 0)
    {
        timer_config.hour = lv_roller_get_selected(timer_screen->roller.hour);
        timer_config.min = lv_roller_get_selected(timer_screen->roller.min);
        timer_config.sec = lv_roller_get_selected(timer_screen->roller.sec);

        timer_config.status = START;
        lv_obj_add_flag(timer_screen->btn.start, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(timer_screen->roller.hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(timer_screen->roller.min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(timer_screen->roller.sec, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_sec, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->btn.pause, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->btn.stop, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->label_time, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text_fmt(timer_screen->label_time, "%02d:%02d:%02d", timer_config.hour, timer_config.min, timer_config.sec);

        label_time_text = lv_label_create(timer_screen->screen);
        lv_obj_set_style_text_font(label_time_text, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(label_time_text, "共%d小时%d分钟%d秒", timer_config.hour, timer_config.min, timer_config.sec);
        lv_obj_align_to(label_time_text, timer_screen->label_time, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        timer_screen->timer = lv_timer_create(timer_cb, 1000, NULL);
        lv_timer_ready(timer_screen->timer);

    } else if (target == timer_screen->btn.pause && timer_config.status == START) {
        timer_config.status = PAUSE;
        lv_img_set_src(btn_img, "A:images/timer/start.bin");
        lv_timer_pause(timer_screen->timer);
    } else if (target == timer_screen->btn.pause && timer_config.status == PAUSE) {
        timer_config.status = START;
        lv_img_set_src(btn_img, "A:images/timer/pause.bin");
        lv_timer_resume(timer_screen->timer);
    } else if (target == timer_screen->btn.stop) {
        timer_config.status = STOP;
        lv_obj_add_flag(timer_screen->btn.pause, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(timer_screen->btn.stop, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(timer_screen->label_time, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->btn.start, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->roller.hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->roller.min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(timer_screen->roller.sec, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_sec, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(btn_img, "A:images/timer/pause.bin");

        lv_roller_set_selected(timer_screen->roller.hour, 0, LV_ANIM_OFF);
        lv_roller_set_selected(timer_screen->roller.min, 0, LV_ANIM_OFF);
        lv_roller_set_selected(timer_screen->roller.sec, 0, LV_ANIM_OFF);

        lv_timer_del(timer_screen->timer);
        lv_obj_del(label_time_text);
    }
}

static void timer_cb(lv_timer_t *timer)
{
    lv_label_set_text_fmt(timer_screen->label_time, "%02d:%02d:%02d", timer_config.hour, timer_config.min,
                          timer_config.sec);

    timer_config.sec--;

    if (timer_config.sec < 0) {
        timer_config.sec = 59;
        timer_config.min--;
        if (timer_config.min < 0) {
            timer_config.min = 59;
            timer_config.hour--;
            if (timer_config.hour < 0) {
                lv_timer_del(timer);
                timer_config.status = STOP;
                lv_obj_t * mbox = lv_msgbox_create(lv_scr_act(), "计时", "计时结束！", NULL, true);
                lv_obj_set_style_text_font(mbox, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_align(mbox, LV_ALIGN_CENTER, 0, 0);
                lv_obj_set_size(mbox, 176, 120);
                if(lv_scr_act() != timer_screen->screen) {
                    lv_obj_del_async(timer_screen->screen);
                    free(timer_screen);
                    timer_screen = NULL;
                }
            }
        }
    }
}

extern menu_screen_t *menu_screen;
static void ui_event_timer_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        lv_obj_set_tile_id(menu_screen->tv, 3, 0, LV_ANIM_OFF);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {

        if(timer_config.status == STOP) {
            lv_obj_del_async(timer_screen->screen);
            free(timer_screen);
            timer_screen = NULL;
        }

    }
}

timer_config_t get_timer_config(void)
{
    return timer_config;
}