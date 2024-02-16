#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "esp_private/esp_psram_extram.h"
#include "main.h"
#include "display.h"
#include "ui.h"
#include "time.h"
#include "wifi.h"
#include "sntp.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("任务：%s 发现栈溢出\n", pcTaskName);
}

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_psram_extram_reserve_dma_pool(163840);
    nvs_init();

    display_init();
    ESP_LOGI("main", "Display LVGL");
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    // wifi_init();
    // time_update();

    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}