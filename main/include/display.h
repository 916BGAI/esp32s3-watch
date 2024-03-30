#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "display.h"

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
#define LCD_DRAW_BUF_HEIGHT     130

/* LCD display color bits */
#define LCD_BITS_PER_PIXEL      (16)

lv_disp_t *display_init(void);

esp_err_t display_brightness_set(uint32_t brightness_percent);
esp_err_t display_brightness_save_to_nvs(void);
uint32_t display_brightness_get(void);
esp_err_t display_brightness_obtain_from_nvs(void);

static inline esp_err_t display_backlight_off(void)
{
    return display_brightness_set(0);
}

static inline esp_err_t display_backlight_on(void)
{
    return display_brightness_set(100);
}

#ifdef __cplusplus
} /*extern "C"*/
#endif