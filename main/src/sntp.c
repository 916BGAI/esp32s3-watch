#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"
#include "nvs_flash.h"
#include "sntp.h"

static const char *TAG = "SNTP";

typedef struct {
    const char *namespace;
    nvs_handle_t handle;
    const char *key;
    uint32_t percent;
} sntp_config_t;

static sntp_config_t sntp_config = {
    .namespace = "sntp",
    .handle = 0,
    .key = "sntp",
};

static void print_servers(void)
{
    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i) {
        ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
    }
}

void time_obtain(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("ntp.aliyun.com");
    config.start = false;
    config.server_from_dhcp = true;
    config.renew_servers_after_new_IP = true;
    config.index_of_first_server = 1;
    config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
    config.smooth_sync = false;
    esp_netif_sntp_init(&config);

    ESP_LOGI(TAG, "Starting SNTP");
    esp_netif_sntp_start();
    print_servers();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
    esp_netif_sntp_deinit();
}

static void set_time_to_nvs(void)
{
    time_t timer;
    time(&timer);
    nvs_open(sntp_config.namespace, NVS_READWRITE, &sntp_config.handle);
    nvs_set_i64(sntp_config.handle, sntp_config.key, timer);
    nvs_commit(sntp_config.handle);
    nvs_close(sntp_config.handle);
}

esp_err_t get_time_from_nvs(void)
{
    setenv("TZ", "CST-8", 1);
    tzset();
    struct timeval tv = { 0 };
    nvs_open(sntp_config.namespace, NVS_READONLY, &sntp_config.handle);
    const esp_err_t err = nvs_get_i64(sntp_config.handle, sntp_config.key, &tv.tv_sec);
    nvs_close(sntp_config.handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        tv.tv_sec = 0;
        settimeofday(&tv, NULL);
        return ESP_FAIL;
    } else {
        settimeofday(&tv, NULL);
        return ESP_OK;
    }
}

void time_update(void)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        time_obtain();
        time(&now);
    } else {
        {
            ESP_LOGI(TAG, "Add a error for test adjtime");
            struct timeval tv_now;
            gettimeofday(&tv_now, NULL);
            int64_t cpu_time = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
            int64_t error_time = cpu_time + 500 * 1000L;
            struct timeval tv_error = { .tv_sec = error_time / 1000000L, .tv_usec = error_time % 1000000L };
            settimeofday(&tv_error, NULL);
        }

        ESP_LOGI(TAG, "Time was set, now just adjusting it. Use SMOOTH SYNC method.");
        time_obtain();
        time(&now);
    }

    char strftime_buf[64];

    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH) {
        struct timeval outdelta;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) {
            adjtime(NULL, &outdelta);
            ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %jd sec: %li ms: %li us",
                     (intmax_t)outdelta.tv_sec, outdelta.tv_usec / 1000, outdelta.tv_usec % 1000);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }

    set_time_to_nvs();
}