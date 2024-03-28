#pragma once

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t fatfs_init(void);
esp_err_t usb_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif