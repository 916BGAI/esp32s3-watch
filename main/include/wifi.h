/**
 * @file wifi.h
 * @brief Wi-Fi模块头文件，包含Wi-Fi的函数声明。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "esp_wifi.h"

esp_err_t wifi_init(void);
esp_err_t wifi_start(void);
esp_err_t wifi_destroy(void);
esp_err_t wifi_get_connected_status(const TickType_t xTicksToWait);
esp_err_t wifi_info_save_to_nvs(void);
esp_err_t wifi_info_obtain_from_nvs(void);
esp_err_t wifi_info_set_status(const bool status);
uint32_t wifi_info_get_status(void);
esp_err_t wifi_info_save_status_to_nvs(void);
esp_err_t wifi_info_set_ssid(const uint8_t *ssid);
const uint8_t * wifi_info_get_ssid(void);
esp_err_t wifi_info_set_pwd(const uint8_t *password);
const uint8_t * wifi_info_get_pwd(void);
esp_err_t wifi_info_set_authmode(const wifi_auth_mode_t authmode);
wifi_auth_mode_t wifi_info_get_authmode(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif