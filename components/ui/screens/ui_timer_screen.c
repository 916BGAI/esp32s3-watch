#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_timer_screen.h"

static timer_screen_t *timer_screen;

static void ui_event_timer_screen(lv_event_t *e);

void ui_timer_screen_init(void)
{
    timer_screen = malloc(sizeof(timer_screen_t));
    timer_screen->screen = lv_obj_create(NULL);

    lv_obj_t * label_text = lv_label_create(timer_screen->screen);
    lv_obj_set_style_text_font(label_text, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_text, "计时");
    lv_obj_align(label_text, LV_ALIGN_DEFAULT, 20, 20);

    timer_screen->roller.hour = lv_roller_create(timer_screen->screen);
    const char* hour_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                               "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
    lv_roller_set_options(timer_screen->roller.hour, hour_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.hour, 3);
    lv_obj_align(timer_screen->roller.hour, LV_ALIGN_TOP_MID, -70, 80);
    lv_obj_set_size(timer_screen->roller.hour, 50, 97);

    timer_screen->roller.min = lv_roller_create(timer_screen->screen);
    const char* min_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                              "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                              "\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36"
                              "\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48"
                              "\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    lv_roller_set_options(timer_screen->roller.min, min_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.min, 3);
    lv_obj_align(timer_screen->roller.min, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_size(timer_screen->roller.min, 50, 97);

    timer_screen->roller.sec = lv_roller_create(timer_screen->screen);
    const char* sec_options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12"
                              "\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"
                              "\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36"
                              "\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48"
                              "\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    lv_roller_set_options(timer_screen->roller.sec, sec_options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(timer_screen->roller.sec, 3);
    lv_obj_align(timer_screen->roller.sec, LV_ALIGN_TOP_MID, 70, 80);
    lv_obj_set_size(timer_screen->roller.sec, 50, 97);

    lv_obj_add_event_cb(timer_screen->screen, ui_event_timer_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(timer_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

extern menu_screen_t *menu_screen;
static void ui_event_timer_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {

        free(timer_screen);
        timer_screen = NULL;
    }
}