#include "lvgl.h"
#include "ui_helpers.h"
#include "esp_lvgl_port.h"
#include "display.h"

extern lv_obj_t *ui_options_screen;
extern lv_obj_t *options_list;
extern lv_obj_t *options_top_contanier;
extern lv_obj_t *options_top_label;
extern lv_obj_t *options_return_button;
extern lv_obj_t *options_save_button;
extern lv_style_t options_style;

static lv_obj_t *brightness_contanier;
static lv_obj_t * slider;
static lv_obj_t *slider_label;
static char slider_label_buf[8];
static lv_style_t style_main;
static lv_style_t style_indicator;
static lv_style_t style_knob;
static lv_style_t style_pressed_color;

static void slider_event_cb(lv_event_t * e)
{
    slider = lv_event_get_target(e);
    lv_snprintf(slider_label_buf, sizeof(slider_label_buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, slider_label_buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

void brightness_event_callback(lv_event_t *e)
{
    lv_obj_add_flag(options_list, LV_OBJ_FLAG_HIDDEN);
    brightness_contanier = lv_obj_create(ui_options_screen);
    lv_obj_set_size(brightness_contanier, 240, 250);
    lv_obj_add_style(brightness_contanier, &options_style, 0);
    lv_obj_align(brightness_contanier, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_load_anim(brightness_contanier, options_list, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 100);
    lv_label_set_text(options_top_label, "屏 幕 亮 度");
    lv_obj_load_anim(options_top_label, options_top_label, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);

    lv_obj_clear_flag(options_return_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(options_save_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_load_anim(options_return_button, options_return_button, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);
    lv_obj_load_anim(options_save_button, options_save_button, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 100);

    /*Create a transition*/
    static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, 0};
    static lv_style_transition_dsc_t transition_dsc;
    lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);

    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
    lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
    lv_style_set_pad_ver(&style_main, -2); /*Makes the indicator larger*/

    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);
    lv_style_set_transition(&style_indicator, &transition_dsc);

    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_CYAN, 3));
    lv_style_set_border_width(&style_knob, 2);
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
    lv_style_set_transition(&style_knob, &transition_dsc);

    lv_style_init(&style_pressed_color);
    lv_style_set_bg_color(&style_pressed_color, lv_palette_darken(LV_PALETTE_CYAN, 2));

    /*Create a slider and add the style*/
    slider = lv_slider_create(brightness_contanier);
    lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
    lv_obj_set_size(slider, 170, 17);
    lv_obj_center(slider);
    slider_label = lv_label_create(brightness_contanier);
    lv_slider_set_value(slider, display_brightness_get(), LV_ANIM_ON);
    lv_snprintf(slider_label_buf, sizeof(slider_label_buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, slider_label_buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_add_style(slider, &style_main, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_KNOB | LV_STATE_PRESSED);
}

void return_save_button_event_callback(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    if (target == options_save_button)
    {
        uint32_t brightness_percent = lv_slider_get_value(slider);
        lvgl_port_lock(0);
        display_brightness_set(brightness_percent);
        lvgl_port_unlock();
    }

    lv_obj_add_flag(options_return_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(options_save_button, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(options_top_label, "设 置");
    lv_obj_load_anim(options_top_label, options_top_label, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100);
    lv_obj_load_anim(options_list, brightness_contanier, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 100);
    lv_style_reset(&style_main);
    lv_style_reset(&style_indicator);
    lv_style_reset(&style_pressed_color);
    lv_style_reset(&style_knob);
    lv_style_reset(&style_pressed_color);
    lv_obj_del(brightness_contanier);
    lv_obj_clear_flag(options_list, LV_OBJ_FLAG_HIDDEN);
}