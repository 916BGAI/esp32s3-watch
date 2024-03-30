#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

esp_err_t wifi_init(void);
esp_err_t wifi_info_save_to_nvs(void);
esp_err_t wifi_info_obtain_from_nvs(void);
esp_err_t wifi_info_set_status(const uint32_t status);
uint32_t wifi_info_get_status(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif