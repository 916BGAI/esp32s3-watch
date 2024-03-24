#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_UI_IMG_HAND_HOUR
#define LV_ATTRIBUTE_IMG_UI_IMG_HAND_HOUR
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_UI_IMG_HAND_HOUR uint8_t ui_img_hand_hour_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x6d, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x49, 0x2c, 0x25, 0xc6, 0x24, 0xf3, 0x49, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x09, 0x00, 0x00, 0x24, 0x09, 0x24, 0x0b, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x05, 0x00, 0x00, 
  0x6e, 0x1d, 0x25, 0xea, 0x00, 0xff, 0x00, 0xff, 0x00, 0xfb, 0x00, 0xfd, 0x24, 0xff, 0x24, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xfe, 0x24, 0xfe, 0x24, 0xfe, 0x24, 0xfe, 0x24, 0xfe, 0x24, 0xfc, 0x24, 0xf7, 0x24, 0xfa, 0x24, 0xf6, 0x24, 0xbd, 0x24, 0x76, 0x24, 0x58, 
  0x00, 0x59, 0x00, 0xff, 0x49, 0x82, 0x6e, 0x4a, 0x00, 0xc4, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x00, 0xfe, 
  0x49, 0xbd, 0x24, 0xe3, 0x49, 0x2d, 0x92, 0x0b, 0x24, 0x49, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 
  0x49, 0x82, 0x24, 0xe7, 0x25, 0x2c, 0x92, 0x0f, 0x49, 0x6e, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x00, 0xfd, 
  0x24, 0x41, 0x00, 0xff, 0x25, 0xff, 0x49, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xfe, 0x00, 0xff, 0x00, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xff, 0x24, 0xfe, 0x24, 0xfa, 0x24, 0xeb, 0x24, 0xe3, 0x24, 0xc6, 0x24, 0xaf, 
  0x92, 0x02, 0x25, 0x72, 0x00, 0xff, 0x24, 0xff, 0x24, 0xe4, 0x24, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x0a, 0x24, 0x1e, 0x24, 0x23, 0x24, 0x26, 0x24, 0x1a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x0b, 0x24, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x01, 0x49, 0x40, 0x49, 0x8f, 0x92, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x0c, 0x63, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x49, 0x4a, 0x2c, 0x04, 0x21, 0xc6, 0xc3, 0x18, 0xf3, 0xe8, 0x41, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0xe4, 0x20, 0x09, 0xc3, 0x18, 0x0b, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 
  0x0c, 0x63, 0x1d, 0x45, 0x29, 0xea, 0x00, 0x00, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xfb, 0x00, 0x00, 0xfd, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x62, 0x10, 0xff, 0x61, 0x08, 0xff, 0x61, 0x08, 0xff, 0x61, 0x08, 0xfe, 0x61, 0x08, 0xfe, 0x41, 0x08, 0xfe, 0x41, 0x08, 0xfe, 0x21, 0x08, 0xff, 0x61, 0x08, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfc, 0xc3, 0x18, 0xf7, 0xc3, 0x18, 0xfa, 0xc3, 0x18, 0xf6, 0xc3, 0x18, 0xbd, 0xc3, 0x18, 0x76, 0xc3, 0x18, 0x58, 
  0x82, 0x10, 0x59, 0x62, 0x10, 0xff, 0x29, 0x4a, 0x82, 0x0c, 0x63, 0x4a, 0x41, 0x08, 0xc4, 0x61, 0x08, 0xff, 0x20, 0x00, 0xff, 0x41, 0x08, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x62, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0xa2, 0x10, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0x82, 0x10, 0xfe, 
  0x29, 0x4a, 0xbd, 0x82, 0x10, 0xe3, 0x08, 0x42, 0x2d, 0xaf, 0x7b, 0x0b, 0xe4, 0x20, 0x49, 0x61, 0x08, 0xff, 0x00, 0x00, 0xff, 0x21, 0x08, 0xff, 0x20, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x21, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x20, 0x00, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x41, 0x08, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x00, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x21, 0x08, 0xff, 0x20, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x21, 0x08, 0xff, 0x21, 0x08, 0xff, 0x20, 0x00, 0xff, 
  0xa6, 0x31, 0x82, 0x82, 0x10, 0xe7, 0x45, 0x29, 0x2c, 0xcf, 0x7b, 0x0f, 0x49, 0x4a, 0x6e, 0x00, 0x00, 0xff, 0x20, 0x00, 0xff, 0x20, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x20, 0x00, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x61, 0x08, 0xff, 0x61, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x62, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x62, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xff, 0xa2, 0x10, 0xff, 0xa2, 0x10, 0xff, 0x82, 0x10, 0xff, 0xa2, 0x10, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xa2, 0x10, 0xff, 0x41, 0x08, 0xfd, 
  0x04, 0x21, 0x41, 0x20, 0x00, 0xff, 0x66, 0x31, 0xff, 0xa7, 0x39, 0xff, 0x20, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x21, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xff, 0x41, 0x08, 0xfe, 0x41, 0x08, 0xff, 0x61, 0x08, 0xff, 0x82, 0x10, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xa2, 0x10, 0xff, 0xa2, 0x10, 0xff, 0xa2, 0x10, 0xfe, 0xa2, 0x10, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xa3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xff, 0xc3, 0x18, 0xfe, 0xc3, 0x18, 0xfa, 0xc3, 0x18, 0xeb, 0xc3, 0x18, 0xe3, 0xc3, 0x18, 0xc6, 0xc3, 0x18, 0xaf, 
  0x10, 0x84, 0x02, 0x66, 0x31, 0x72, 0x61, 0x08, 0xff, 0x82, 0x10, 0xff, 0x82, 0x10, 0xe4, 0xe4, 0x20, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x18, 0x0a, 0xe3, 0x18, 0x1e, 0xc3, 0x18, 0x23, 0xe3, 0x18, 0x26, 0xa3, 0x18, 0x1a, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x18, 0x0b, 0xe3, 0x18, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x21, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x29, 0x4a, 0x40, 0x08, 0x42, 0x8f, 0x51, 0x8c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x63, 0x0c, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x4a, 0x49, 0x2c, 0x21, 0x04, 0xc6, 0x18, 0xc3, 0xf3, 0x41, 0xe8, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x20, 0xe4, 0x09, 0x18, 0xc3, 0x0b, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 
  0x63, 0x0c, 0x1d, 0x29, 0x45, 0xea, 0x00, 0x00, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xfb, 0x00, 0x00, 0xfd, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x62, 0xff, 0x08, 0x61, 0xff, 0x08, 0x61, 0xff, 0x08, 0x61, 0xfe, 0x08, 0x61, 0xfe, 0x08, 0x41, 0xfe, 0x08, 0x41, 0xfe, 0x08, 0x21, 0xff, 0x08, 0x61, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfc, 0x18, 0xc3, 0xf7, 0x18, 0xc3, 0xfa, 0x18, 0xc3, 0xf6, 0x18, 0xc3, 0xbd, 0x18, 0xc3, 0x76, 0x18, 0xc3, 0x58, 
  0x10, 0x82, 0x59, 0x10, 0x62, 0xff, 0x4a, 0x29, 0x82, 0x63, 0x0c, 0x4a, 0x08, 0x41, 0xc4, 0x08, 0x61, 0xff, 0x00, 0x20, 0xff, 0x08, 0x41, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x62, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0xa2, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xa3, 0xff, 0x10, 0x82, 0xfe, 
  0x4a, 0x29, 0xbd, 0x10, 0x82, 0xe3, 0x42, 0x08, 0x2d, 0x7b, 0xaf, 0x0b, 0x20, 0xe4, 0x49, 0x08, 0x61, 0xff, 0x00, 0x00, 0xff, 0x08, 0x21, 0xff, 0x00, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x08, 0x21, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x00, 0x20, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x08, 0x41, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x08, 0x21, 0xff, 0x00, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x08, 0x21, 0xff, 0x08, 0x21, 0xff, 0x00, 0x20, 0xff, 
  0x31, 0xa6, 0x82, 0x10, 0x82, 0xe7, 0x29, 0x45, 0x2c, 0x7b, 0xcf, 0x0f, 0x4a, 0x49, 0x6e, 0x00, 0x00, 0xff, 0x00, 0x20, 0xff, 0x00, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x20, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x08, 0x61, 0xff, 0x08, 0x61, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x10, 0x62, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x62, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xff, 0x10, 0xa2, 0xff, 0x10, 0xa2, 0xff, 0x10, 0x82, 0xff, 0x10, 0xa2, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x10, 0xa2, 0xff, 0x08, 0x41, 0xfd, 
  0x21, 0x04, 0x41, 0x00, 0x20, 0xff, 0x31, 0x66, 0xff, 0x39, 0xa7, 0xff, 0x00, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x08, 0x21, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xff, 0x08, 0x41, 0xfe, 0x08, 0x41, 0xff, 0x08, 0x61, 0xff, 0x10, 0x82, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x10, 0xa2, 0xff, 0x10, 0xa2, 0xff, 0x10, 0xa2, 0xfe, 0x10, 0xa2, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xa3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xff, 0x18, 0xc3, 0xfe, 0x18, 0xc3, 0xfa, 0x18, 0xc3, 0xeb, 0x18, 0xc3, 0xe3, 0x18, 0xc3, 0xc6, 0x18, 0xc3, 0xaf, 
  0x84, 0x10, 0x02, 0x31, 0x66, 0x72, 0x08, 0x61, 0xff, 0x10, 0x82, 0xff, 0x10, 0x82, 0xe4, 0x20, 0xe4, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xe3, 0x0a, 0x18, 0xe3, 0x1e, 0x18, 0xc3, 0x23, 0x18, 0xe3, 0x26, 0x18, 0xa3, 0x1a, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xc3, 0x0b, 0x18, 0xe3, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x4a, 0x29, 0x40, 0x42, 0x08, 0x8f, 0x8c, 0x51, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 32
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x01, 0x5e, 0x5e, 0x5e, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x46, 0x46, 0x46, 0x2c, 0x21, 0x21, 0x21, 0xc6, 0x16, 0x16, 0x16, 0xf3, 0x3c, 0x3c, 0x3c, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x1c, 0x09, 0x17, 0x17, 0x17, 0x0b, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 
  0x61, 0x61, 0x61, 0x1d, 0x29, 0x29, 0x29, 0xea, 0x01, 0x01, 0x01, 0xff, 0x04, 0x04, 0x04, 0xff, 0x05, 0x05, 0x05, 0xfb, 0x01, 0x01, 0x01, 0xfd, 0x11, 0x11, 0x11, 0xff, 0x10, 0x10, 0x10, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0c, 0x0c, 0x0c, 0xff, 0x0b, 0x0b, 0x0b, 0xff, 0x0b, 0x0b, 0x0b, 0xff, 0x0b, 0x0b, 0x0b, 0xfe, 0x0b, 0x0b, 0x0b, 0xfe, 0x09, 0x09, 0x09, 0xfe, 0x08, 0x08, 0x08, 0xfe, 0x05, 0x05, 0x05, 0xff, 0x0a, 0x0a, 0x0a, 0xff, 0x17, 0x17, 0x17, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfc, 0x18, 0x18, 0x18, 0xf7, 0x18, 0x18, 0x18, 0xfa, 0x18, 0x18, 0x18, 0xf6, 0x18, 0x18, 0x18, 0xbd, 0x18, 0x18, 0x18, 0x76, 0x17, 0x17, 0x17, 0x58, 
  0x0e, 0x0e, 0x0e, 0x59, 0x0c, 0x0c, 0x0c, 0xff, 0x45, 0x45, 0x45, 0x82, 0x60, 0x60, 0x60, 0x4a, 0x08, 0x08, 0x08, 0xc4, 0x0a, 0x0a, 0x0a, 0xff, 0x02, 0x02, 0x02, 0xff, 0x07, 0x07, 0x07, 0xff, 0x11, 0x11, 0x11, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xff, 0x11, 0x11, 0x11, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xff, 0x12, 0x12, 0x12, 0xff, 0x17, 0x17, 0x17, 0xff, 0x16, 0x16, 0x16, 0xff, 0x16, 0x16, 0x16, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x16, 0x16, 0x16, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x14, 0x14, 0x14, 0xff, 0x0f, 0x0f, 0x0f, 0xfe, 
  0x45, 0x45, 0x45, 0xbd, 0x11, 0x11, 0x11, 0xe3, 0x3e, 0x3e, 0x3e, 0x2d, 0x74, 0x74, 0x74, 0x0b, 0x1c, 0x1c, 0x1c, 0x49, 0x0a, 0x0a, 0x0a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x04, 0x04, 0x04, 0xff, 0x02, 0x02, 0x02, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x05, 0x05, 0x05, 0xff, 0x07, 0x07, 0x07, 0xff, 0x06, 0x06, 0x06, 0xff, 0x06, 0x06, 0x06, 0xff, 0x06, 0x06, 0x06, 0xff, 0x03, 0x03, 0x03, 0xff, 0x04, 0x04, 0x04, 0xff, 0x05, 0x05, 0x05, 0xff, 0x04, 0x04, 0x04, 0xff, 0x05, 0x05, 0x05, 0xff, 0x07, 0x07, 0x07, 0xff, 0x05, 0x05, 0x05, 0xff, 0x05, 0x05, 0x05, 0xff, 0x05, 0x05, 0x05, 0xff, 0x03, 0x03, 0x03, 0xff, 0x03, 0x03, 0x03, 0xff, 0x03, 0x03, 0x03, 0xff, 0x03, 0x03, 0x03, 0xff, 0x03, 0x03, 0x03, 0xff, 0x03, 0x03, 0x03, 0xff, 0x02, 0x02, 0x02, 0xff, 0x02, 0x02, 0x02, 0xff, 0x01, 0x01, 0x01, 0xff, 0x02, 0x02, 0x02, 0xff, 0x02, 0x02, 0x02, 0xff, 0x03, 0x03, 0x03, 0xff, 0x04, 0x04, 0x04, 0xff, 0x03, 0x03, 0x03, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x05, 0x05, 0x05, 0xff, 0x04, 0x04, 0x04, 0xff, 0x02, 0x02, 0x02, 0xff, 
  0x33, 0x33, 0x33, 0x82, 0x11, 0x11, 0x11, 0xe7, 0x29, 0x29, 0x29, 0x2c, 0x77, 0x77, 0x77, 0x0f, 0x46, 0x46, 0x46, 0x6e, 0x00, 0x00, 0x00, 0xff, 0x02, 0x02, 0x02, 0xff, 0x03, 0x03, 0x03, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x00, 0x00, 0x00, 0xff, 0x03, 0x03, 0x03, 0xff, 0x08, 0x08, 0x08, 0xff, 0x09, 0x09, 0x09, 0xff, 0x0a, 0x0a, 0x0a, 0xff, 0x0b, 0x0b, 0x0b, 0xff, 0x09, 0x09, 0x09, 0xff, 0x08, 0x08, 0x08, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x11, 0x11, 0x11, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0d, 0x0d, 0x0d, 0xff, 0x0e, 0x0e, 0x0e, 0xff, 0x11, 0x11, 0x11, 0xff, 0x10, 0x10, 0x10, 0xff, 0x11, 0x11, 0x11, 0xff, 0x13, 0x13, 0x13, 0xff, 0x13, 0x13, 0x13, 0xff, 0x11, 0x11, 0x11, 0xff, 0x13, 0x13, 0x13, 0xff, 0x14, 0x14, 0x14, 0xff, 0x15, 0x15, 0x15, 0xff, 0x15, 0x15, 0x15, 0xff, 0x16, 0x16, 0x16, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x17, 0x17, 0x17, 0xff, 0x16, 0x16, 0x16, 0xff, 0x17, 0x17, 0x17, 0xff, 0x13, 0x13, 0x13, 0xff, 0x06, 0x06, 0x06, 0xfd, 
  0x1f, 0x1f, 0x1f, 0x41, 0x03, 0x03, 0x03, 0xff, 0x2d, 0x2d, 0x2d, 0xff, 0x35, 0x35, 0x35, 0xff, 0x03, 0x03, 0x03, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0xff, 0x05, 0x05, 0x05, 0xff, 0x07, 0x07, 0x07, 0xff, 0x07, 0x07, 0x07, 0xff, 0x07, 0x07, 0x07, 0xfe, 0x06, 0x06, 0x06, 0xff, 0x0b, 0x0b, 0x0b, 0xff, 0x10, 0x10, 0x10, 0xff, 0x15, 0x15, 0x15, 0xff, 0x14, 0x14, 0x14, 0xff, 0x15, 0x15, 0x15, 0xff, 0x17, 0x17, 0x17, 0xff, 0x16, 0x16, 0x16, 0xfe, 0x17, 0x17, 0x17, 0xff, 0x16, 0x16, 0x16, 0xff, 0x13, 0x13, 0x13, 0xff, 0x13, 0x13, 0x13, 0xff, 0x13, 0x13, 0x13, 0xfe, 0x13, 0x13, 0x13, 0xff, 0x14, 0x14, 0x14, 0xff, 0x14, 0x14, 0x14, 0xff, 0x14, 0x14, 0x14, 0xff, 0x15, 0x15, 0x15, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0xfe, 0x18, 0x18, 0x18, 0xfa, 0x18, 0x18, 0x18, 0xeb, 0x18, 0x18, 0x18, 0xe3, 0x18, 0x18, 0x18, 0xc6, 0x17, 0x17, 0x17, 0xaf, 
  0x80, 0x80, 0x80, 0x02, 0x2d, 0x2d, 0x2d, 0x72, 0x0b, 0x0b, 0x0b, 0xff, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0xe4, 0x1d, 0x1d, 0x1d, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x1a, 0x1a, 0x0a, 0x1a, 0x1a, 0x1a, 0x1e, 0x16, 0x16, 0x16, 0x23, 0x1b, 0x1b, 0x1b, 0x26, 0x14, 0x14, 0x14, 0x1a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x17, 0x17, 0x0b, 0x1a, 0x1a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x01, 0x44, 0x44, 0x44, 0x40, 0x40, 0x40, 0x40, 0x8f, 0x88, 0x88, 0x88, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
#endif
};

const lv_img_dsc_t ui_img_hand_hour = {
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 50,
  .header.h = 9,
  .data_size = 450 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .data = ui_img_hand_hour_map,
};