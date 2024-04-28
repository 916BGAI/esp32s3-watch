/**
 * @file ui.c
 * @brief UI初始化功能的实现。
 */

#include "ui.h"

/**
 * @brief UI中文本元素的字体。
 */
lv_font_t *SarasaMonoR_16;      /**< 16像素 普通 */
lv_font_t *SarasaMonoR_18;      /**< 18像素 普通 */
lv_font_t *SarasaMonoB_18;      /**< 18像素 粗体 */
lv_font_t *SarasaMonoB_30;      /**< 30像素 粗体 */

/**
 * @brief 初始化用户界面。
 * 
 * 此函数初始化LittlevGL库，设置显示主题，并初始化特定的UI元素。
 */
void ui_init(void)
{
    lv_font_init();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp,
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              false, SarasaMonoR_18);
    lv_disp_set_theme(dispp, theme);
    ui_clock_screen_init();     // 初始化时钟界面UI
}