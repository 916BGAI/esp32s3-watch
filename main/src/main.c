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
#include "sntp.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("任务：%s 发现栈溢出\n", pcTaskName);
}

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);

    nvs_init();
    fatfs_init();
    wifi_init();

    ESP_LOGI("main", "free_internal_heap_size = %ldKB", esp_get_free_internal_heap_size() / 1024);
    ESP_LOGI("main", "free_heap_size = %ldKB", esp_get_free_heap_size() / 1024);

    display_init();
    ESP_LOGI("main", "Display LVGL");
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    // time_update();

    ESP_LOGI("main", "free_internal_heap_size = %ldKB", esp_get_free_internal_heap_size() / 1024);
    ESP_LOGI("main", "free_heap_size = %ldKB", esp_get_free_heap_size() / 1024);

    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}