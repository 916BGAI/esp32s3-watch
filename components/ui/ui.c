#include "ui.h"

void ui_init(void)
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp,
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              false, &ui_font_sarasa16);
    lv_disp_set_theme(dispp, theme);
    ui_clock_screen_init();
}