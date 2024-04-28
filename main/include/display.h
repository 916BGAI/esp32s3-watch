/**
 * @file display.h
 * @brief 显示屏控制器头文件。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "display.h"

/**
 * @defgroup Display 显示器控制
 * @brief 用于控制显示器和亮度的函数和常量。
 */

/**@{*/

/* Display */
#define LCD_PCLK                (GPIO_NUM_18)           /**< LCD时钟引脚 */
#define LCD_DATA0               (GPIO_NUM_17)           /**< LCD数据引脚 */
#define LCD_CS                  (GPIO_NUM_16)           /**< LCD片选引脚 */
#define LCD_RST                 (GPIO_NUM_15)           /**< LCD复位引脚 */
#define LCD_DC                  (GPIO_NUM_14)           /**< LCD数据/命令选择引脚 */
#define LCD_BACKLIGHT           (GPIO_NUM_13)           /**< LCD背光控制引脚 */

#define LCD_CMD_BITS            8                       /**< LCD命令位数 */
#define LCD_PARAM_BITS          8                       /**< LCD参数位数 */
#define LCD_LEDC_CH             1                       /**< LCD背光控制的LEDC通道 */

#define LCD_H_RES               (240)                   /**< LCD横向分辨率 */
#define LCD_V_RES               (280)                   /**< LCD纵向分辨率 */
#define LCD_PIXEL_CLOCK_HZ      (80 * 1000 * 1000)      /**< LCD时钟频率 */
#define LCD_SPI_NUM             (SPI2_HOST)             /**< LCD SPI总线编号 */
#define LCD_DRAW_BUF_HEIGHT     120                     /**< LCD绘制缓冲区高度 */

#define LCD_BITS_PER_PIXEL      (16)                    /**< LCD显示颜色的位数 */

lv_disp_t *display_init(void);
esp_err_t display_brightness_set(uint32_t brightness_percent);
esp_err_t display_brightness_save_to_nvs(void);
uint32_t display_brightness_get(void);
esp_err_t display_brightness_obtain_from_nvs(void);

/**
 * @brief 关闭显示屏背光。
 * 
 * @return esp_err_t 如果成功，则返回ESP_OK，否则返回错误代码。
 */
static inline esp_err_t display_backlight_off(void)
{
    return display_brightness_set(0);
}

/**
 * @brief 打开显示屏背光。
 * 
 * @return esp_err_t 如果成功，则返回ESP_OK，否则返回错误代码。
 */
static inline esp_err_t display_backlight_on(void)
{
    return display_brightness_set(100);
}

/**@}*/

#ifdef __cplusplus
} /*extern "C"*/
#endif