#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"
#include "sntp.h"

static const char *TAG = "SNTP";

static void print_servers(void)
{
    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
    }
}

void obtain_time(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("ntp.aliyun.com");
    config.start = false;
    config.server_from_dhcp = true;
    config.renew_servers_after_new_IP = true;
    config.index_of_first_server = 1;
    config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
    config.smooth_sync = true;
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
