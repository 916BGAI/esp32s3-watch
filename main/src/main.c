#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "main.h"
#include "wifi.h"
#include "clock.h"
#include "display.h"
#include "ui.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("任务：%s 发现栈溢出\n", pcTaskName);
}

extern void example_lvgl_demo_ui(lv_obj_t *scr);

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    nvs_init();
    // wifi_init();
    // time_update();

    display_init();
    ESP_LOGI("main", "Display LVGL");
    display_backlight_on();
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    // static char pcWriteBuffer[512] = {0};

    for (;;) {
        // vTaskList(pcWriteBuffer);
        // printf("-----------------------------------------\n");
        // printf("Name            State   Priority    Stack   Num\n");
        // printf("%s\n", pcWriteBuffer);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}