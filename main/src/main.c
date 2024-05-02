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
    // esp_log_level_set("*", ESP_LOG_INFO);
    // static char buffer[256];

    nvs_init();
    fatfs_init();
    wifi_init();

    // sprintf(buffer, "   Biggest /     Free /    Total\n"
    //             "\t  SRAM : [%8dKB / %8dKB / %8dKB]\n"
    //             "\t PSRAM : [%8dKB / %8dKB / %8dKB]",
    //             heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) / 1024,
    //             heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024,
    //             heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024,
    //             heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024,
    //             heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024,
    //             heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024);
    // ESP_LOGI("MEM", "%s", buffer);

    display_init();
    ESP_LOGI("main", "Display LVGL");
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    for (;;) {
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // const char *pcHeader = "任务名称\t任务优先级\t堆栈使用量\t任务ID\t任务状态\r\n";
        // char cTaskList[256]; /* 存储任务列表的缓冲区 */
        // vTaskList((char *) &cTaskList);
        // ESP_LOGI("TASK", "%s\n", pcHeader);
        // ESP_LOGI("TASK", "%s\n", cTaskList);
        //
        // sprintf(buffer, "   Biggest /     Free /    Total\n"
        //         "\t  SRAM : [%8dKB / %8dKB / %8dKB]\n"
        //         "\t PSRAM : [%8dKB / %8dKB / %8dKB]",
        //         heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) / 1024,
        //         heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024,
        //         heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024,
        //         heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024,
        //         heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024,
        //         heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024);
        // ESP_LOGI("MEM", "%s", buffer);
    }
}