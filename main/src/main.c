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

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("任务：%s 发现栈溢出\n", pcTaskName);
}

static void led_init(void)
{
    static const gpio_config_t led_io_conf = {
        .intr_type = GPIO_INTR_DISABLE,         // 禁用中断
        .mode = GPIO_MODE_OUTPUT,               // 输出模式
        .pin_bit_mask = (1ULL << GPIO_NUM_10),  // GPIO10
        .pull_down_en = 0,                      // 禁用下拉
        .pull_up_en = 0,                        // 禁用上拉
    };

    gpio_config(&led_io_conf);
}

static void led_task(void *arg)
{
    uint32_t led_status = 0;

    for (;;)
    {
        led_status = !led_status;
        gpio_set_level(GPIO_NUM_10, led_status);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

extern void example_lvgl_demo_ui(lv_obj_t *scr);

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    nvs_init();
    // wifi_init();
    // time_update();
    led_init();

    display_init();
    ESP_LOGI("example", "Display LVGL animation");
    display_backlight_on();
    lvgl_port_lock(0);
    lv_demo_widgets();
    lvgl_port_unlock();

    xTaskCreate(led_task, "LED Task", 2048, NULL, 1, NULL);

    static char pcWriteBuffer[512] = {0};

    for (;;)
    {
        vTaskList(pcWriteBuffer);
        printf("-----------------------------------------\n");
        printf("Name            State   Priority    Stack   Num\n");
        printf("%s\n", pcWriteBuffer);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
