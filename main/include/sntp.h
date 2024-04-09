#pragma once

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t set_time_to_nvs(void);
esp_err_t get_time_from_nvs(void);
esp_err_t time_update(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif