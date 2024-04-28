/**
 * @file touch.h
 * @brief 包含了触摸屏相关函数的声明。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t touch_new(esp_lcd_touch_handle_t *ret_touch);

#ifdef __cplusplus
} /*extern "C"*/
#endif