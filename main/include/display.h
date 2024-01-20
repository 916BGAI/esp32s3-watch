#pragma once

#include "lvgl.h"
#include "lv_demos.h"
#include "esp_lvgl_port.h"

/* Display */
#define LCD_PCLK                (GPIO_NUM_18)
#define LCD_DATA0               (GPIO_NUM_17)
#define LCD_CS                  (GPIO_NUM_16)
#define LCD_RST                 (GPIO_NUM_15)
#define LCD_DC                  (GPIO_NUM_14)
#define LCD_BACKLIGHT           (GPIO_NUM_13)

#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8
#define LCD_LEDC_CH             1                   // range 0 7

#define LCD_H_RES               (240)
#define LCD_V_RES               (280)
#define LCD_PIXEL_CLOCK_HZ      (80 * 1000 * 1000)
#define LCD_SPI_NUM             (SPI2_HOST)
#define LCD_DRAW_BUF_HEIGHT     240                 //range 10 240

/* LCD display color format */
#define LCD_COLOR_FORMAT        (ESP_LCD_COLOR_FORMAT_RGB565)
/* LCD display color bytes endianess */
#define LCD_BIGENDIAN           (0)
/* LCD display color bits */
#define LCD_BITS_PER_PIXEL      (16)
/* LCD display color space */
#define LCD_COLOR_SPACE         (ESP_LCD_COLOR_SPACE_RGB)

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
