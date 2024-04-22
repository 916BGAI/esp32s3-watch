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

#define LOCATION_ZHENGZHOU               "101180101"
#define LOCATION_BEIJING                 "101010100"
#define LOCATION_NEWYORK                 "1E98E"

#define _OPTION_                        "location=101020100&gzip=n&lang=zh"
#define _OPTION_MULTI                   "location=%s&lang=zh"

#define WEATHER_SERVER      "devapi.qweather.com"
#define WEB_URL_NOW         "https://" WEATHER_SERVER "/v7/weather/now?" _OPTION_MULTI "&" _KEY_
#define WEB_URL_AIR         "https://" WEATHER_SERVER "/v7/air/now?"     _OPTION_ "&" _KEY_

typedef struct {
    int id;
    const char *icon;
} weather_icon_t;

static weather_screen_t *weather_screen;
static const char *TAG = "app_weather";
static weather_info_t *weather_info[LOCATION_NUM_MAX];
static char client_data [MAX_HTTP_RECV_BUFFER];

static void* spiram_malloc(size_t size);
static voidpf zalloc(voidpf opaque, uInt items, uInt size);
static void zfree(voidpf opaque, voidpf address);
static void weather_reg(location_num_t location, lv_obj_t *tile);
static esp_err_t weather_start(void);
static esp_err_t weather_request(location_num_t location);
static esp_err_t weather_parse_now(char *buffer, location_num_t location);
static int network_gzip_decompress(void *in_buf, size_t in_size, void *out_buf, size_t *out_size, size_t out_buf_size);
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
    weather_request(LOCATION_NUM_ZHENGZHOU);
    weather_request(LOCATION_NUM_BEIJING);
    weather_request(LOCATION_NUM_NEWYORK);

    weather_screen->screen = lv_obj_create(NULL);
    weather_screen->tv = lv_tileview_create(weather_screen->screen);
    lv_obj_set_style_bg_color(weather_screen->tv, lv_color_hex(0xDDE4FB), 0);
    lv_obj_set_style_bg_grad_color(weather_screen->tv, lv_color_hex(0xFDE6CD), 0);
    lv_obj_set_style_bg_grad_dir(weather_screen->tv, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_bg_opa(weather_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(weather_screen->tv, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_font(weather_screen->tv, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    weather_screen->tile1 = lv_tileview_add_tile(weather_screen->tv, 0, 0, LV_DIR_RIGHT);
    weather_screen->tile2 = lv_tileview_add_tile(weather_screen->tv, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    weather_screen->tile3 = lv_tileview_add_tile(weather_screen->tv, 2, 0, LV_DIR_LEFT);

    lv_obj_t * location_label = lv_label_create(weather_screen->tile1);
    lv_obj_set_style_text_font(location_label, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "郑州市");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 20, 31);
    location_label = lv_label_create(weather_screen->tile1);
    lv_obj_set_style_text_color(location_label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(location_label, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "河南省");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 122, 43);

    location_label = lv_label_create(weather_screen->tile2);
    lv_obj_set_style_text_font(location_label, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "北京市");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 20, 31);
    location_label = lv_label_create(weather_screen->tile2);
    lv_obj_set_style_text_color(location_label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(location_label, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "北京市");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 122, 43);

    location_label = lv_label_create(weather_screen->tile3);
    lv_obj_set_style_text_font(location_label, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "纽约");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 20, 31);
    location_label = lv_label_create(weather_screen->tile3);
    lv_obj_set_style_text_color(location_label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_font(location_label, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(location_label, "美国");
    lv_obj_align(location_label, LV_ALIGN_DEFAULT, 93, 43);

    weather_reg(LOCATION_NUM_ZHENGZHOU, weather_screen->tile1);
    weather_reg(LOCATION_NUM_BEIJING, weather_screen->tile2);
    weather_reg(LOCATION_NUM_NEWYORK, weather_screen->tile3);

    lv_obj_add_event_cb(weather_screen->screen, ui_event_weather_screen, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(weather_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
}

static void* spiram_malloc(size_t size)
{
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

static voidpf zalloc(voidpf opaque, uInt items, uInt size)
{
    (void)opaque;
    return heap_caps_malloc(items * size, MALLOC_CAP_SPIRAM);
}

static void zfree(voidpf opaque, voidpf address)
{
    (void)opaque;
    free(address);
}

static void weather_reg(location_num_t location, lv_obj_t *tile)
{
    weather_screen->label.obsTime = lv_label_create(tile);
    lv_obj_set_style_text_font(weather_screen->label.obsTime, SarasaMonoR_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(weather_screen->label.obsTime, weather_info[location]->obsTime);
    lv_obj_align(weather_screen->label.obsTime, LV_ALIGN_DEFAULT, 83, 6);

    weather_screen->label.temp = lv_label_create(tile);
    lv_obj_set_style_text_font(weather_screen->label.temp, SarasaMonoB_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(weather_screen->label.temp, "%d°", weather_info[location]->temp);
    lv_obj_align(weather_screen->label.temp, LV_ALIGN_DEFAULT, 163, 98);

    weather_screen->label.describe = lv_label_create(tile);
    lv_label_set_long_mode(weather_screen->label.describe, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(weather_screen->label.describe, SarasaMonoB_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(weather_screen->label.describe, weather_info[location]->describe);
    lv_obj_set_width(weather_screen->label.describe, 39);
    lv_obj_align(weather_screen->label.describe, LV_ALIGN_DEFAULT, 163, 136);

    char weather_icon_src[25] = {0};
    sprintf(weather_icon_src, "A:images/weather/%d.bin", weather_info[location]->icon_code);
    weather_screen->weather_icon = lv_img_create(tile);
    lv_img_set_src(weather_screen->weather_icon, weather_icon_src);
    lv_obj_align(weather_screen->weather_icon, LV_ALIGN_DEFAULT, 23, 78);

    lv_obj_t *contanier = lv_obj_create(tile);
    lv_obj_set_style_bg_color(contanier, lv_color_hex(0xA8B4C5), LV_PART_MAIN);
    lv_obj_set_style_border_width(contanier, 0, 0);
    lv_obj_set_style_opa(contanier, 70, 0);
    lv_obj_clear_flag(contanier, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(contanier, 197, 79);
    lv_obj_align(contanier, LV_ALIGN_BOTTOM_MID, 0, -20);

    weather_screen->label.windScale = lv_label_create(tile);
    lv_obj_set_style_text_font(weather_screen->label.windScale, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(weather_screen->label.windScale, "%d级", weather_info[location]->windScale);
    lv_obj_align_to(weather_screen->label.windScale, contanier, LV_ALIGN_TOP_LEFT, 23, 2);

    weather_screen->label.windDir = lv_label_create(tile);
    lv_obj_set_style_text_color(weather_screen->label.windDir, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_style_text_font(weather_screen->label.windDir, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(weather_screen->label.windDir, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(weather_screen->label.windDir, weather_info[location]->windDir);
    lv_obj_set_style_text_align(weather_screen->label.windDir, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_width(weather_screen->label.windDir, 56);
    lv_obj_align_to(weather_screen->label.windDir, weather_screen->label.windScale, LV_ALIGN_OUT_BOTTOM_MID, 0, 7);

    weather_screen->label.humidity = lv_label_create(tile);
    lv_obj_set_style_text_font(weather_screen->label.humidity, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(weather_screen->label.humidity, "%d%%", weather_info[location]->humidity);
    lv_obj_align_to(weather_screen->label.humidity, contanier, LV_ALIGN_TOP_RIGHT, -26, 2);

    lv_obj_t *humidity_text = lv_label_create(tile);
    lv_obj_set_style_text_color(humidity_text, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_style_text_font(humidity_text, SarasaMonoR_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(humidity_text, "相对湿度");
    lv_obj_align_to(humidity_text, weather_screen->label.humidity, LV_ALIGN_OUT_BOTTOM_MID, 0, 7);
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

            weather_info[i]->obsTime = heap_caps_malloc(17 * sizeof(char), MALLOC_CAP_SPIRAM);
            CHECK(weather_info[i]->obsTime, "Failed allocate mem", ESP_ERR_NO_MEM);
            weather_info[i]->describe = heap_caps_malloc(25 * sizeof(char), MALLOC_CAP_SPIRAM);
            CHECK(weather_info[i]->describe, "Failed allocate mem", ESP_ERR_NO_MEM);
            weather_info[i]->windDir = heap_caps_malloc(16 * sizeof(char), MALLOC_CAP_SPIRAM);
            CHECK(weather_info[i]->windDir, "Failed allocate mem", ESP_ERR_NO_MEM);

            weather_info[i]->temp = 0;
            weather_info[i]->icon_code = 999;
            weather_info[i]->humidity = 0;
            weather_info[i]->windScale = 0;
            strcpy(weather_info[i]->describe, "N/A");
            strcpy(weather_info[i]->obsTime, "N/A");
            strcpy(weather_info[i]->windDir, "N/A");
        }
    }

    return ESP_OK;
}

static esp_err_t weather_request(location_num_t location)
{
    char *cityID = NULL;

    char *url_request = heap_caps_malloc(200, MALLOC_CAP_SPIRAM);
    CHECK(url_request, "Failed allocate mem", ESP_ERR_NO_MEM);

    switch (location) {
    case LOCATION_NUM_ZHENGZHOU:
        cityID = LOCATION_ZHENGZHOU;
        break;
    case LOCATION_NUM_BEIJING:
        cityID = LOCATION_BEIJING;
        break;
    case LOCATION_NUM_NEWYORK:
        cityID = LOCATION_NEWYORK;
        break;
    default:
        cityID = LOCATION_ZHENGZHOU;
        break;
    }

    sprintf(url_request, WEB_URL_NOW, cityID);
    ESP_LOGI(TAG, "url_request:%s\r\n", url_request);

    esp_http_client_config_t config = {
        .url = url_request,
        .method = HTTP_METHOD_GET,
        .buffer_size = MAX_HTTP_RECV_BUFFER,
        .crt_bundle_attach=esp_crt_bundle_attach,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Host", WEATHER_SERVER);
    esp_http_client_set_header(client, "User-Agent", _UA_);
    esp_http_client_set_header(client, "Accept-Encoding", "deflate");
    esp_http_client_set_header(client, "Cache-Control", "no-cache");
    esp_http_client_set_header(client, "Accept", "*/*");

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

    int data_read = esp_http_client_read_response(client, client_data, MAX_HTTP_RECV_BUFFER);
    if (data_read >= 0) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
        esp_http_client_get_status_code(client),
        (int)esp_http_client_get_content_length(client));
        size_t out_len = 0;
        char *gzip_out_buff = heap_caps_malloc(MAX_HTTP_RECV_BUFFER * sizeof(char), MALLOC_CAP_SPIRAM);
        memset(gzip_out_buff, 0x0, MAX_HTTP_RECV_BUFFER);
        err = network_gzip_decompress(client_data, data_read, gzip_out_buff, &out_len, MAX_HTTP_RECV_BUFFER);
        if(err!= ESP_OK) {
            ESP_LOGE(TAG, "gzip data decompression failed, code=%d\n",err);
        }
        memcpy(client_data,gzip_out_buff,out_len);
        free(gzip_out_buff);
        gzip_out_buff = NULL;
        ESP_LOGI("TAG", "%s", client_data);
        weather_parse_now(client_data, location);
    }

    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    if (url_request) {
        free(url_request);
        url_request = NULL;
    }

    return ESP_OK;
}

static esp_err_t weather_parse_now(char *buffer, location_num_t location)
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
            cJSON *json_item_obsTime = cJSON_GetObjectItem(json_now, "obsTime");
            cJSON *json_item_windScale = cJSON_GetObjectItem(json_now, "windScale");
            cJSON *json_item_windDir = cJSON_GetObjectItem(json_now, "windDir");

            printf("Temp :     [%s]\n", json_item_temp->valuestring);
            printf("Icon :     [%s]\n", json_item_icon->valuestring);
            printf("Text :     [%s]\n", json_item_text->valuestring);
            printf("Humidity:  [%s]\n", json_item_humidity->valuestring);
            printf("obsTime:   [%s]\n", json_item_obsTime->valuestring);
            printf("windScale: [%s]\n", json_item_windScale->valuestring);
            printf("windDir:   [%s]\n", json_item_windDir->valuestring);

            weather_info[location]->temp = atoi(json_item_temp->valuestring);
            weather_info[location]->icon_code = atoi(json_item_icon->valuestring);
            weather_info[location]->humidity = atoi(json_item_humidity->valuestring);
            weather_info[location]->windScale = atoi(json_item_windScale->valuestring);

            memset(weather_info[location]->obsTime, 0, 17 * sizeof(char));
            memset(weather_info[location]->describe, 0, 25 * sizeof(char));
            memset(weather_info[location]->windDir, 0, 16 * sizeof(char));

            strcpy(weather_info[location]->describe, json_item_text->valuestring);
            strcpy(weather_info[location]->windDir, json_item_windDir->valuestring);
            strncpy(weather_info[location]->obsTime, json_item_obsTime->valuestring, 10);
            strcat(weather_info[location]->obsTime, " ");
            strncat(weather_info[location]->obsTime, json_item_obsTime->valuestring + 11, 5);
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

static int network_gzip_decompress(void *in_buf, size_t in_size, void *out_buf, size_t *out_size, size_t out_buf_size)
{
    int err;
    z_stream d_stream = {0}; /* decompression stream */
    d_stream.zalloc = zalloc;
    d_stream.zfree = zfree;
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

extern menu_screen_t *menu_screen;
static void ui_event_weather_screen(lv_event_t *e)
{
    const lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_menu_screen_init();
        lv_scr_load_anim(menu_screen->screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
        lv_obj_set_tile_id(menu_screen->tv, 2, 0, LV_ANIM_OFF);
    } else if (event_code == LV_EVENT_SCREEN_UNLOADED) {
        for (int i = 0; i < LOCATION_NUM_MAX; i++) {
            free(weather_info[i]->windDir);
            weather_info[i]->windDir = NULL;
            free(weather_info[i]->describe);
            weather_info[i]->describe = NULL;
            free(weather_info[i]->obsTime);
            weather_info[i]->obsTime = NULL;
            free(weather_info[i]);
            weather_info[i] = NULL;
        }

        free(weather_screen);
        weather_screen = NULL;
    }
}