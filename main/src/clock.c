#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "sntp.h"
#include "lvgl/lvgl.h"
#include "clock.h"

static const char *TAG = "CLOCK";
