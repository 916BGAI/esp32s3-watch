/**
 * @file wifi.c
 * @brief Wi-Fi模块实现文件，包含Wi-Fi的初始化、启动、销毁、状态获取和配置保存等功能函数。
 */

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

static esp_netif_t *netif;

typedef struct {
    const char *namespace;
    nvs_handle_t handle;

    struct {
        const char *status;
        const char *ssid;
        const char *password;
        const char *authmode;
    } key;

    bool status;
    uint8_t ssid[32];
    uint8_t password[64];
    wifi_auth_mode_t authmode;

} wifi_info_t;

static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "WIFI";

static wifi_info_t wifi_info = {
    .namespace = "wifi",
    .handle = 0,
    .key = {
        .status = "status",
        .ssid = "ssid",
        .password = "password",
        .authmode = "authmode",
    },
};

/**
 * @brief Wi-Fi事件处理函数
 * 
 * @param arg 参数指针
 * @param event_base 事件基础
 * @param event_id 事件ID
 * @param event_data 事件数据指针
 */
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

/**
 * @brief Wi-Fi初始化函数
 * 
 * @return esp_err_t 初始化结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_init(void)
{
    s_wifi_event_group = xEventGroupCreate();
    return ESP_OK;
}

/**
 * @brief Wi-Fi启动函数
 * 
 * @return esp_err_t 启动结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_start(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = { 0 };
    memcpy(wifi_config.sta.ssid, wifi_info.ssid, 32);
    memcpy(wifi_config.sta.password, wifi_info.password, 64);
    wifi_config.sta.threshold.authmode = wifi_info.authmode;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    wifi_get_connected_status(portMAX_DELAY);

    return ESP_OK;
}

/**
 * @brief Wi-Fi销毁函数
 * 
 * @return esp_err_t 销毁结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_destroy(void)
{
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();

    if(wifi_get_connected_status(0) == ESP_OK) {
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (wifi_get_connected_status(0) == ESP_FAIL) {
        xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }

    return ESP_OK;
}

/**
 * @brief 获取Wi-Fi连接状态
 * 
 * @param xTicksToWait 等待时间
 * @return esp_err_t 连接状态，ESP_OK 表示连接成功，ESP_FAIL 表示连接失败，ESP_ERR_NOT_FOUND 表示未找到连接状态
 */
esp_err_t wifi_get_connected_status(const TickType_t xTicksToWait)
{
    const EventBits_t bits =
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, xTicksToWait);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", (char *)wifi_info.ssid, (char *)wifi_info.password);
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", (char *)wifi_info.ssid, (char *)wifi_info.password);
        return ESP_FAIL;
    } else {
        ESP_LOGI(TAG, "WIFI don't connected");
        return ESP_ERR_NOT_FOUND;
    }
}

/**
 * @brief 将Wi-Fi配置信息保存到NVS中
 * 
 * @return esp_err_t 保存结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_save_to_nvs(void)
{
    nvs_open(wifi_info.namespace, NVS_READWRITE, &wifi_info.handle);
    nvs_set_u32(wifi_info.handle, wifi_info.key.status, wifi_info.status);
    nvs_set_str(wifi_info.handle, wifi_info.key.ssid, (const char *)wifi_info.ssid);
    nvs_set_str(wifi_info.handle, wifi_info.key.password, (const char *)wifi_info.password);
    nvs_set_u32(wifi_info.handle, wifi_info.key.authmode, wifi_info.authmode);
    nvs_commit(wifi_info.handle);
    nvs_close(wifi_info.handle);
    return ESP_OK;
}

/**
 * @brief 从NVS中获取Wi-Fi配置信息
 * 
 * @return esp_err_t 获取结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_obtain_from_nvs(void)
{
    size_t ssid_size = 32;
    size_t pwd_size = 64;
    nvs_open(wifi_info.namespace, NVS_READONLY, &wifi_info.handle);
    ESP_ERROR_CHECK(nvs_get_u32(wifi_info.handle, wifi_info.key.status, (uint32_t *)&wifi_info.status));
    ESP_ERROR_CHECK(nvs_get_str(wifi_info.handle, wifi_info.key.ssid, (char *)wifi_info.ssid, &ssid_size));
    ESP_ERROR_CHECK(nvs_get_str(wifi_info.handle, wifi_info.key.password, (char *)wifi_info.password, &pwd_size));
    ESP_ERROR_CHECK(nvs_get_u32(wifi_info.handle, wifi_info.key.authmode, (uint32_t *)&wifi_info.authmode));
    nvs_close(wifi_info.handle);
    return ESP_OK;
}

/**
 * @brief 设置Wi-Fi连接状态
 * 
 * @param status 连接状态，true 表示已连接，false 表示未连接
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_set_status(const bool status)
{
    wifi_info.status = status;
    return ESP_OK;
}

/**
 * @brief 获取Wi-Fi连接状态
 * 
 * @return uint32_t 连接状态，true 表示已连接，false 表示未连接
 */
uint32_t wifi_info_get_status(void)
{
    return wifi_info.status;
}

/**
 * @brief 将Wi-Fi状态信息保存到NVS中
 * 
 * @return esp_err_t 保存结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_save_status_to_nvs(void)
{
    nvs_open(wifi_info.namespace, NVS_READWRITE, &wifi_info.handle);
    nvs_set_u32(wifi_info.handle, wifi_info.key.status, wifi_info.status);
    nvs_commit(wifi_info.handle);
    nvs_close(wifi_info.handle);
    return ESP_OK;
}

/**
 * @brief 设置Wi-Fi SSID
 * 
 * @param ssid SSID字符串
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_set_ssid(const uint8_t *ssid)
{
    memcpy(wifi_info.ssid, ssid, 32);
    return ESP_OK;
}

/**
 * @brief 获取Wi-Fi SSID
 * 
 * @return const uint8_t* SSID字符串
 */
const uint8_t * wifi_info_get_ssid(void)
{
    return wifi_info.ssid;
}

/**
 * @brief 设置Wi-Fi密码
 * 
 * @param password 密码字符串
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_set_pwd(const uint8_t *password)
{
    memcpy(wifi_info.password, password, 64);
    return ESP_OK;
}

/**
 * @brief 获取Wi-Fi密码
 * 
 * @return const uint8_t* 密码字符串
 */
const uint8_t * wifi_info_get_pwd(void)
{
    return wifi_info.password;
}

/**
 * @brief 设置Wi-Fi认证模式
 * 
 * @param authmode 认证模式
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败
 */
esp_err_t wifi_info_set_authmode(const wifi_auth_mode_t authmode)
{
    wifi_info.authmode = authmode;
    return ESP_OK;
}

/**
 * @brief 获取Wi-Fi认证模式
 * 
 * @return wifi_auth_mode_t 认证模式
 */
wifi_auth_mode_t wifi_info_get_authmode(void)
{
    return wifi_info.authmode;
}