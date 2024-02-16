#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void time_obtain(void);
esp_err_t get_time_from_nvs(void);
void time_update(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif