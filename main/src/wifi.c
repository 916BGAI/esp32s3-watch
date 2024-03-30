#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"

#define MAXIMUM_RETRY  5
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

typedef struct {
    const char *namespace;
    nvs_handle_t handle;

    struct {
        const char *status;
    } key;

    uint32_t status;
} wifi_info_t;

static uint8_t wifi_ssid[32] = "Redmi";
static uint8_t wifi_pass[64] = "20011201ABCabc";
static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "WIFI";

static wifi_info_t wifi_info = {
    .namespace = "wifi",
    .handle = 0,
    .key = {
        .status = "status",
    },
    .status = false,
};

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t wifi_init(void)
{
    s_wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = { 0 };
    memcpy(wifi_config.sta.ssid, wifi_ssid, 32);
    memcpy(wifi_config.sta.password, wifi_pass, 64);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    EventBits_t bits =
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", wifi_ssid, wifi_pass);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", wifi_ssid, wifi_pass);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    return ESP_OK;
}

esp_err_t wifi_info_save_to_nvs(void)
{
    nvs_open(wifi_info.namespace, NVS_READWRITE, &wifi_info.handle);
    nvs_set_u32(wifi_info.handle, wifi_info.key.status, wifi_info.status);
    nvs_commit(wifi_info.handle);
    nvs_close(wifi_info.handle);
    return ESP_OK;
}

esp_err_t wifi_info_obtain_from_nvs(void)
{
    nvs_open(wifi_info.namespace, NVS_READONLY, &wifi_info.handle);
    const esp_err_t err = nvs_get_u32(wifi_info.handle, wifi_info.key.status, &wifi_info.status);
    nvs_close(wifi_info.handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("WIFI", "Can't find wifi from nvs");
        return ESP_FAIL;
    } else {
        return ESP_OK;
    }
}

esp_err_t wifi_info_set_status(const uint32_t status)
{
    wifi_info.status = status;
    return ESP_OK;
}

uint32_t wifi_info_get_status(void)
{
    return wifi_info.status;
}