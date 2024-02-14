#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    char *name;
    const lv_img_dsc_t *img_src;
    lv_obj_t *(*entry_point)(void *user_date);
} application_info_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif