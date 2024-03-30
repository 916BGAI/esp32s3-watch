#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

esp_err_t fatfs_init(void);
esp_err_t usb_init(void);
esp_err_t usb_deinit(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif