#pragma once

#include "lvgl.h"
#include "lv_demos.h"
#include "esp_lvgl_port.h"

esp_err_t display_brightness_set(int brightness_percent);
lv_disp_t *display_init(void);

static inline esp_err_t display_backlight_off(void)
{
    return display_brightness_set(0);
}

static inline esp_err_t display_backlight_on(void)
{
    return display_brightness_set(100);
}
