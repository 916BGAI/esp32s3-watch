#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "nvs_flash.h"

static inline void nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

#ifdef __cplusplus
} /*extern "C"*/
#endif