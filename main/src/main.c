#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "main.h"
#include "display.h"
#include "ui.h"
#include "usb.h"
#include "wifi.h"

void app_main(void)
{
    nvs_init();
    fatfs_init();
    wifi_init();

    display_init();
    ESP_LOGI("main", "Display LVGL");
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    for (;;) {
        vTaskDelay(700 / portTICK_PERIOD_MS);
    }
}