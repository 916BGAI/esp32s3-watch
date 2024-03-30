#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"
#include "nvs_flash.h"
#include "touch.h"
#include "display.h"

#define TAG "DISPLAY"

static lv_disp_t *disp;
static lv_indev_t *disp_indev;
static esp_lcd_touch_handle_t tp;

typedef struct {
    const char *namespace;
    nvs_handle_t handle;
    const char *key;
    uint32_t percent;
} brightness_config_t;

static brightness_config_t brightness_config = {
    .namespace = "brightness",
    .handle = 0,
    .key = "brightness",
    .percent = 0
};

esp_err_t display_brightness_set(uint32_t brightness_percent)
{
    brightness_config.percent = brightness_percent;

    ESP_LOGI("DISPLAY", "Setting LCD backlight: %lu%%", brightness_percent);
    const uint32_t duty_cycle = (1023 * brightness_percent) / 100; // LEDC resolution set to 10bits, thus: 100% = 1023
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));

    return ESP_OK;
}

esp_err_t display_brightness_save_to_nvs(void)
{
    nvs_open(brightness_config.namespace, NVS_READWRITE, &brightness_config.handle);
    nvs_set_u32(brightness_config.handle, brightness_config.key, brightness_config.percent);
    nvs_commit(brightness_config.handle);
    nvs_close(brightness_config.handle);

    return ESP_OK;
}

uint32_t display_brightness_get(void)
{
    return brightness_config.percent;
}

esp_err_t display_brightness_obtain_from_nvs(void)
{
    nvs_open(brightness_config.namespace, NVS_READONLY, &brightness_config.handle);
    const esp_err_t err = nvs_get_u32(brightness_config.handle, brightness_config.key, &brightness_config.percent);
    nvs_close(brightness_config.handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("DISPLAY", "Can't find brightness from nvs");
        return ESP_FAIL;
    } else {
        return ESP_OK;
    }
}

static esp_err_t display_brightness_init(void)
{
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = LCD_BACKLIGHT,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = 1,
        .duty = 0,
        .hpoint = 0,
    };

    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = 1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));

    esp_err_t err = display_brightness_obtain_from_nvs();
    if (err == ESP_FAIL) {
        display_backlight_on();
    } else {
        ESP_LOGI(TAG, "Setting LCD backlight: %lu%%", brightness_config.percent);
        uint32_t duty_cycle =
            (1023 * brightness_config.percent) / 100; // LEDC resolution set to 10bits, thus: 100% = 1023
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));
    }
    return ESP_OK;
}

static esp_err_t display_new(const int max_transfer_sz, esp_lcd_panel_handle_t *ret_panel,
                             esp_lcd_panel_io_handle_t *ret_io)
{
    esp_err_t ret = ESP_OK;
    assert(max_transfer_sz > 0);

    ESP_RETURN_ON_ERROR(display_brightness_init(), TAG, "Brightness init failed");

    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_PCLK,
        .mosi_io_num = LCD_DATA0,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = max_transfer_sz,
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    ESP_LOGD(TAG, "Install panel IO");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_NUM, &io_config, ret_io), err, TAG,
                      "New panel IO failed");

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST, // Shared with Touch reset
        .rgb_ele_order = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(*ret_io, &panel_config, ret_panel), err, TAG, "New panel failed");

    esp_lcd_panel_reset(*ret_panel);
    esp_lcd_panel_init(*ret_panel);
    esp_lcd_panel_invert_color(*ret_panel, true);
    return ret;

err:
    if (*ret_panel) {
        esp_lcd_panel_del(*ret_panel);
    }
    if (*ret_io) {
        esp_lcd_panel_io_del(*ret_io);
    }
    spi_bus_free(LCD_SPI_NUM);
    return ret;
}

static lv_disp_t *display_lcd_init(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    const int max_transfer_sz = (LCD_H_RES * LCD_DRAW_BUF_HEIGHT) * sizeof(uint16_t);
    ESP_ERROR_CHECK(display_new(max_transfer_sz, &panel_handle, &io_handle));

    esp_lcd_panel_disp_on_off(panel_handle, true);

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_H_RES * LCD_DRAW_BUF_HEIGHT,
        .double_buffer = 1,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = false,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
        },
    };

    return lvgl_port_add_disp(&disp_cfg);
}

static lv_indev_t *display_indev_init(lv_disp_t *disp)
{
    ESP_ERROR_CHECK(touch_new(&tp));
    assert(tp);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = disp,
        .handle = tp,
    };

    return lvgl_port_add_touch(&touch_cfg);
}

lv_disp_t *display_init(void)
{
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
    ESP_ERROR_CHECK(display_brightness_init());
    disp = display_lcd_init();
    disp_indev = display_indev_init(disp);

    return disp;
}