#include "cJSON.h"
#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_more_screen.h"

static more_screen_t *more_screen;

char* json_get_version(char *jsonData, const char *string);
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
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 303);

    more_screen->label.flash = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.flash, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.flash, "闪存 " UI_SYMBOL_FLASH);
    more_screen->label.flash = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.flash, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.flash, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.flash, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.flash, "16MB");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 386);

    more_screen->label.resolution = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.resolution, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.resolution, "分辨率 " UI_SYMBOL_RESOLUTION);
    more_screen->label.resolution = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.resolution, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.resolution, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.resolution, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.resolution, "240x280");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 469);

    more_screen->label.version.espidf = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.version.espidf, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.version.espidf, "ESP-IDF 版本 " UI_SYMBOL_ESPIDF);
    more_screen->label.version.espidf = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.version.espidf, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.version.espidf, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.version.espidf, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.version.espidf, "N/A");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 552);

    more_screen->label.version.freertos = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.version.freertos, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.version.freertos, "FreeRTOS 版本 " UI_SYMBOL_FREERTOS);
    more_screen->label.version.freertos = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.version.freertos, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.version.freertos, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.version.freertos, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.version.freertos, "N/A");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 635);

    more_screen->label.version.lvgl = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.version.lvgl, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.version.lvgl, "LVGL 版本 ");
    more_screen->label.version.lvgl = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.version.lvgl, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.version.lvgl, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.version.lvgl, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.version.lvgl, "N/A");

    more_screen->contanier =  lv_obj_create(more_screen->screen);
    lv_obj_set_style_border_width(more_screen->contanier, 1, 0);
    lv_obj_set_style_radius(more_screen->contanier, 16, LV_PART_MAIN);
    lv_obj_set_style_bg_color(more_screen->contanier, lv_color_white(), 0);
    lv_obj_set_style_text_font(more_screen->contanier, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(more_screen->contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(more_screen->contanier, 200, 73);
    lv_obj_align(more_screen->contanier, LV_ALIGN_TOP_MID, 0, 718);

    more_screen->label.version.soft = lv_label_create(more_screen->contanier);
    lv_obj_align(more_screen->label.version.soft, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(more_screen->label.version.soft, "软件版本 " UI_SYMBOL_VERSION);
    more_screen->label.version.soft = lv_label_create(more_screen->contanier);
    lv_obj_set_style_text_line_space(more_screen->label.version.soft, 6, LV_PART_MAIN);
    lv_obj_set_style_text_font(more_screen->label.version.soft, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(more_screen->label.version.soft, LV_ALIGN_TOP_LEFT, 0, 27);
    lv_label_set_text(more_screen->label.version.soft, "N/A");

    lv_fs_file_t file_p;
    lv_fs_res_t fs_res = lv_fs_open(&file_p, "A:version.json", LV_FS_MODE_RD);
    if (fs_res != LV_FS_RES_OK) {
        LV_LOG_USER("Open file error!\n");
    }

    uint32_t fileSize = 0;
    lv_fs_seek(&file_p, 0, LV_FS_SEEK_END);
    lv_fs_tell(&file_p, &fileSize);
    lv_fs_seek(&file_p, 0, LV_FS_SEEK_SET);

    char *jsonData = malloc(fileSize + 1);
    lv_fs_read(&file_p, jsonData, fileSize, NULL);
    jsonData[fileSize] = '\0';

    lv_fs_close(&file_p);

    lv_label_set_text_fmt(more_screen->label.version.espidf, "%s", json_get_version(jsonData, "esp-idf"));
    lv_label_set_text_fmt(more_screen->label.version.freertos, "%s", json_get_version(jsonData, "freertos"));
    lv_label_set_text_fmt(more_screen->label.version.lvgl, "%s", json_get_version(jsonData, "lvgl"));
    lv_label_set_text_fmt(more_screen->label.version.soft, "%s", json_get_version(jsonData, NULL));

    lv_obj_add_event_cb(more_screen->screen, ui_event_more_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(more_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

char* json_get_version(char *jsonData, const char *string)
{
    static char valuestring[10] = {0};
    cJSON *root = cJSON_Parse(jsonData);
    cJSON *item = NULL;

    if (root != NULL && string == NULL) {
        const cJSON *version = cJSON_GetObjectItem(root, "version");
        strcpy(valuestring, version->valuestring);
        cJSON_Delete(root);
        return valuestring;
    } else if (root != NULL) {
        item = cJSON_GetObjectItem(root, string);
        if (item != NULL) {
            const cJSON *version = cJSON_GetObjectItem(item, "version");
            strcpy(valuestring, version->valuestring);
        } else {
            LV_LOG_USER("Error parsing object - [%s] - [%d]", __FILE__, __LINE__);
            cJSON_Delete(root);
            return NULL;
        }
        cJSON_Delete(root);
        return valuestring;
    }

    LV_LOG_USER("Error parsing object - [%s] - [%d]", __FILE__, __LINE__);
    cJSON_Delete(root);
    return NULL;
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