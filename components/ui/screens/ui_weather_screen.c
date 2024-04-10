#include "../ui.h"
#include "ui_menu_screen.h"
#include "ui_weather_screen.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "zlib.h"
#include "zutil.h"
#include "cJSON.h"

#define LOG_TRACE(...) ESP_LOGI(TAG, ##__VA_ARGS__)
#define CHECK(a, str, ret_val)                                        \
    do {                                                              \
        if (!(a)) {                                                   \
            ESP_LOGE(TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
        }                                                             \
    } while (0)

#define MAX_HTTP_RECV_BUFFER            (1024)
#define _UA_                            "esp32s3"
#define QWEATHER_REQUEST_KEY_LEN        (32)
#define _KEY_VALUE_                     "140d681da1cc4978be5a17cd8af9f125"
#define _KEY_                           "key=" _KEY_VALUE_

#define LOCATION_SHANGHAI               "101020100"
#define LOCATION_BEIJING                "101010100"
#define LOCATION_SHENZHEN               "101280601"

#define _OPTION_                        "location=101020100&gzip=n&lang=zh"
#define _OPTION_MULTI                   "location=%s&lang=en"

#define WEATHER_SERVER      "devapi.qweather.com"
#define WEB_URL_NOW         "https://" WEATHER_SERVER "/v7/weather/now?" _OPTION_MULTI "&" _KEY_
#define WEB_URL_AIR         "https://" WEATHER_SERVER "/v7/air/now?"     _OPTION_ "&" _KEY_

static weather_screen_t *weather_screen;
static const char *TAG = "app_weather";
static weather_info_t *weather_info[LOCATION_NUM_MAX];

static void* spiram_malloc(size_t size);
static esp_err_t weather_start(void);
esp_err_t weather_request(location_num_t location);
esp_err_t response_handler(esp_http_client_event_t *evt);
static int network_gzip_decompress(void *in_buf, size_t in_size, void *out_buf, size_t *out_size, size_t out_buf_size);
static esp_err_t app_weather_parse_now(char *buffer, location_num_t location);
static void ui_event_weather_screen(lv_event_t *e);

void ui_weather_screen_init(void)
{
    weather_screen = malloc(sizeof(weather_screen_t));

    cJSON_Hooks hooks = {
        .malloc_fn = &spiram_malloc,
        .free_fn   = &free,
    };
    cJSON_InitHooks(&hooks);

    weather_start();
    weather_request(LOCATION_NUM_SHANGHAI);

    weather_screen->screen = lv_obj_create(NULL);

    lv_obj_add_event_cb(weather_screen->screen, ui_event_weather_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(weather_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

static void* spiram_malloc(size_t size)
{
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

static esp_err_t weather_start(void)
{
    CHECK(QWEATHER_REQUEST_KEY_LEN == strlen(_KEY_VALUE_),
          "Invalid QWeather request key.\n"
          "Please register at https://console.qweather.com/#/apps",
          ESP_ERR_INVALID_ARG);

    for (int i = 0; i < LOCATION_NUM_MAX; i++) {
        if (NULL == weather_info[i]) {
            weather_info[i] = heap_caps_malloc(sizeof(weather_info_t), MALLOC_CAP_SPIRAM);
            CHECK(weather_info[i], "Failed allocate mem", ESP_ERR_NO_MEM);

            weather_info[i]->describe = heap_caps_malloc(16, MALLOC_CAP_SPIRAM);
            CHECK(weather_info[i]->describe, "Failed allocate mem", ESP_ERR_NO_MEM);

            weather_info[i]->temp = 20;
            weather_info[i]->icon_code = 104;
            weather_info[i]->humid = 80;
            strcpy(weather_info[i]->describe, "N/A");
        }
    }

    return ESP_OK;
}

esp_err_t weather_request(location_num_t location)
{
    esp_http_client_config_t config = {
        .url = "https://devapi.qweather.com/v7/weather/now?location=101010100&key=140d681da1cc4978be5a17cd8af9f125",
        .method = HTTP_METHOD_GET,
        .crt_bundle_attach=esp_crt_bundle_attach,
        .disable_auto_redirect = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Open HTTP connection success");
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0) {
        ESP_LOGE(TAG, "HTTP client fetch headers failed");
    } else {
        ESP_LOGI(TAG, "HTTP client fetch headers success");
    }

#define MAX_HTTP_OUTPUT_BUFFER  (2048)
    static char client_data [MAX_HTTP_OUTPUT_BUFFER];

    int data_read = esp_http_client_read_response(client, client_data, MAX_HTTP_OUTPUT_BUFFER);
    if (data_read >= 0) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
        esp_http_client_get_status_code(client),
        (int)esp_http_client_get_content_length(client));
        size_t out_len = 0;
        char *gzip_out_buff = malloc(MAX_HTTP_OUTPUT_BUFFER * sizeof(char));
        memset(gzip_out_buff, 0x0, MAX_HTTP_OUTPUT_BUFFER);
        err=network_gzip_decompress(client_data,data_read,gzip_out_buff,&out_len,MAX_HTTP_OUTPUT_BUFFER);
        if(err!= ESP_OK) {
            ESP_LOGE(TAG, "gzip data decompression failed, code=%d\n",err);
        }
        memcpy(client_data,gzip_out_buff,out_len);
        free(gzip_out_buff);
        ESP_LOGI("TAG", "%s", client_data);
        app_weather_parse_now(client_data, LOCATION_NUM_SHANGHAI);
    }

    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    return ESP_OK;
}

esp_err_t response_handler(esp_http_client_event_t *evt)
{
    return ESP_OK;
}

static int network_gzip_decompress(void *in_buf, size_t in_size, void *out_buf, size_t *out_size, size_t out_buf_size)
{
    int err;
    z_stream d_stream = {0}; /* decompression stream */
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in  = in_buf;
    d_stream.avail_in = 0;
    d_stream.next_out = out_buf;

    if ((err = inflateInit2(&d_stream, 47)) != Z_OK) {
        return err;
    }
    while (d_stream.total_out < out_buf_size - 1 && d_stream.total_in < in_size) {
        d_stream.avail_in = d_stream.avail_out = 1;
        if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) {
            break;
        }
        if (err != Z_OK) {
            return err;
        }
    }

    if ((err = inflateEnd(&d_stream)) != Z_OK) {
        return err;
    }

    *out_size = d_stream.total_out;
    ((char *)out_buf)[*out_size] = '\0';

    return Z_OK;
}

static esp_err_t app_weather_parse_now(char *buffer, location_num_t location)
{
    cJSON *json = cJSON_Parse(buffer);
    cJSON *json_now = NULL;

    if (NULL != json) {
        json_now = cJSON_GetObjectItem(json, "now");
        if (NULL != json_now) {
            cJSON *json_item_temp = cJSON_GetObjectItem(json_now, "temp");
            cJSON *json_item_icon = cJSON_GetObjectItem(json_now, "icon");
            cJSON *json_item_text = cJSON_GetObjectItem(json_now, "text");
            cJSON *json_item_humidity = cJSON_GetObjectItem(json_now, "humidity");

            ESP_LOGI(TAG, "Temp : [%s]", json_item_temp->valuestring);
            ESP_LOGI(TAG, "Icon : [%s]", json_item_icon->valuestring);
            ESP_LOGI(TAG, "Text : [%s]", json_item_text->valuestring);
            ESP_LOGI(TAG, "Humid: [%s]", json_item_humidity->valuestring);

            weather_info[location]->temp = atoi(json_item_temp->valuestring);
            weather_info[location]->icon_code = atoi(json_item_icon->valuestring);
            weather_info[location]->humid = atoi(json_item_humidity->valuestring);
            strcpy(weather_info[location]->describe, json_item_text->valuestring);
        } else {
            ESP_LOGE(TAG, "Error parsing object - [%s] - [%d]", __FILE__, __LINE__);
            return ESP_FAIL;
        }
        cJSON_Delete(json);
    } else {
        ESP_LOGE(TAG, "Error parsing object - [%s] - [%d]", __FILE__, __LINE__);
        return ESP_FAIL;
    }

    return ESP_OK;
}

extern menu_screen_t *menu_screen;
void ui_event_weather_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        free(weather_screen);
    }
}