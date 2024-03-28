#include "ui.h"

lv_font_t *SarasaMonoR_16;
lv_font_t *SarasaMonoR_18;
lv_font_t *SarasaMonoB_18;

void ui_init(void)
{
    lv_font_init();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp,
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              false, SarasaMonoR_18);
    lv_disp_set_theme(dispp, theme);
    ui_clock_screen_init();
}