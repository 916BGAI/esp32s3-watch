#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_more_screen.h"

static more_screen_t *more_screen;

static void ui_event_more_screen(lv_event_t *e);

void ui_more_screen_init(void)
{
    more_screen = malloc(sizeof(more_screen_t));
    more_screen->screen = lv_obj_create(NULL);

    more_screen->icon = lv_img_create(more_screen->screen);
    lv_img_set_src(more_screen->icon, "A:images/more/espressif.bin");
    lv_obj_align(more_screen->icon, LV_ALIGN_TOP_MID, 0, 30);

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 127);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 80);

    more_screen->label.cpu = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.cpu, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.cpu, "处理器 " UI_SYMBOL_CPU);
    more_screen->label.cpu = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.cpu, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.cpu, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.cpu, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.cpu, "ESP32-S3\nXtensa® 32位 LX7双核\n最高 240MHz");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 76);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 217);

    more_screen->label.mem = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.mem, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.mem, "内存 " UI_SYMBOL_MEM);
    more_screen->label.mem = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.mem, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.mem, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.mem, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.mem, "512K SRAM + 8MB PSRAM");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 69);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 313);

    more_screen->label.flash = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.flash, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.flash, "闪存 " UI_SYMBOL_FLASH);
    more_screen->label.flash = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.flash, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.flash, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.flash, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.flash, "16MB");

    lv_obj_add_event_cb(more_screen->screen, ui_event_more_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(more_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

extern menu_screen_t *menu_screen;
static void ui_event_more_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
        lv_obj_set_tile_id(menu_screen->tv, 5, 0, LV_ANIM_OFF);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        free(more_screen);
        more_screen = NULL;
    }
}