/**
 * @file lc76g.c
 * @brief lc76g初始化功能的实现。
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lc76g.h"

#define CONFIG_NMEA_PARSER_RING_BUFFER_SIZE 2048
#define NMEA_PARSER_RUNTIME_BUFFER_SIZE (CONFIG_NMEA_PARSER_RING_BUFFER_SIZE / 2)
#define NMEA_MAX_STATEMENT_ITEM_LENGTH (16)
#define NMEA_EVENT_LOOP_QUEUE_SIZE (16)

ESP_EVENT_DEFINE_BASE(ESP_NMEA_EVENT);

#define TIME_ZONE (+8)   //Beijing Time
#define YEAR_BASE (2000) //date in GPS starts from 2000

static const char *TAG = "GNSS";

typedef struct {
    uint8_t item_pos;                              /*!< Current position in item */
    uint8_t item_num;                              /*!< Current item number */
    uint8_t asterisk;                              /*!< Asterisk detected flag */
    uint8_t crc;                                   /*!< Calculated CRC value */
    uint8_t parsed_statement;                      /*!< OR'd of statements that have been parsed */
    uint8_t sat_num;                               /*!< Satellite number */
    uint8_t sat_count;                             /*!< Satellite count */
    uint8_t cur_statement;                         /*!< Current statement ID */
    uint32_t all_statements;                       /*!< All statements mask */
    char item_str[NMEA_MAX_STATEMENT_ITEM_LENGTH]; /*!< Current item */
    gps_t parent;                                  /*!< Parent class */
    uart_port_t uart_port;                         /*!< Uart port number */
    uint8_t *buffer;                               /*!< Runtime buffer */
    esp_event_loop_handle_t event_loop_hdl;        /*!< Event loop handle */
    TaskHandle_t tsk_hdl;                          /*!< NMEA Parser task handle */
    QueueHandle_t event_queue;                     /*!< UART event queue handle */
} lc76g_gnss_t;

static nmea_parser_handle_t nmea_hdl;
static gps_t gps;

static void lc76g_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static nmea_parser_handle_t nmea_parser_init_uart(const nmea_parser_config_t *config);
static esp_err_t nmea_parser_deinit_uart(nmea_parser_handle_t nmea_hdl);
static inline uint8_t convert_two_digit2number(const char *digit_char);
static void parse_utc_time(lc76g_gnss_t *lc76g_gnss);
static float parse_lat_long(lc76g_gnss_t *lc76g_gnss);
static esp_err_t parse_item(lc76g_gnss_t *lc76g_gnss);
static esp_err_t gps_decode(lc76g_gnss_t *lc76g_gnss, size_t len);
static void esp_handle_uart_pattern(lc76g_gnss_t *lc76g_gnss);
static esp_err_t nmea_parser_add_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler, void *handler_args);
static esp_err_t nmea_parser_remove_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler);
static void lc76g_task(void *arg);

esp_err_t lc76g_init(void)
{
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
    nmea_hdl = nmea_parser_init_uart(&config);
    nmea_parser_add_handler(nmea_hdl, lc76g_event_handler, NULL);

    return ESP_OK;
}

esp_err_t lc76g_deinit(void)
{
    nmea_parser_remove_handler(nmea_hdl, lc76g_event_handler);
    nmea_parser_deinit_uart(nmea_hdl);

    return ESP_OK;
}

gps_t lc76g_get_data(void)
{
    return gps;
}

/**
 * @brief GPS Event Handler
 *
 * @param event_handler_arg handler specific arguments
 * @param event_base event base, here is fixed to ESP_NMEA_EVENT
 * @param event_id event id
 * @param event_data event specific arguments
 */
static void lc76g_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id) {
    case GPS_UPDATE:
        gps = *(gps_t *)event_data;
        break;
    case GPS_UNKNOWN:
        /* print unknown statements */
            ESP_LOGW(TAG, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}

/**
 * @brief Init NMEA Parser Use Uart
 *
 * @param config Configuration of NMEA Parser
 * @return nmea_parser_handle_t handle of NMEA parser
 */
static nmea_parser_handle_t nmea_parser_init_uart(const nmea_parser_config_t *config)
{
    lc76g_gnss_t *lc76g_gnss = calloc(1, sizeof(lc76g_gnss_t));
    if (lc76g_gnss == NULL) {
        ESP_LOGE(TAG, "calloc memory for esp_fps failed");
        goto err_gps;
    }

    lc76g_gnss->buffer = calloc(1, NMEA_PARSER_RUNTIME_BUFFER_SIZE);
    if (lc76g_gnss->buffer == NULL) {
        ESP_LOGE(TAG, "calloc memory for runtime buffer failed");
        goto err_buffer;
    }

#if NMEA_STATEMENT_GSA
    lc76g_gnss->all_statements |= (1 << STATEMENT_GSA);
#endif
#if NMEA_STATEMENT_GSV
    lc76g_gnss->all_statements |= (1 << STATEMENT_GSV);
#endif
#if NMEA_STATEMENT_GGA
    lc76g_gnss->all_statements |= (1 << STATEMENT_GGA);
#endif
#if NMEA_STATEMENT_RMC
    lc76g_gnss->all_statements |= (1 << STATEMENT_RMC);
#endif
#if NMEA_STATEMENT_GLL
    lc76g_gnss->all_statements |= (1 << STATEMENT_GLL);
#endif
#if NMEA_STATEMENT_VTG
    lc76g_gnss->all_statements |= (1 << STATEMENT_VTG);
#endif

    /* Set attributes */
    lc76g_gnss->uart_port = config->uart.uart_port;
    lc76g_gnss->all_statements &= 0xFE;

    /* Install UART friver */
    uart_config_t uart_config = {
        .baud_rate = config->uart.baud_rate,
        .data_bits = config->uart.data_bits,
        .parity = config->uart.parity,
        .stop_bits = config->uart.stop_bits,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    if (uart_driver_install(lc76g_gnss->uart_port, CONFIG_NMEA_PARSER_RING_BUFFER_SIZE, 0,
                            config->uart.event_queue_size, &lc76g_gnss->event_queue, 0) != ESP_OK) {
        ESP_LOGE(TAG, "install uart driver failed");
        goto err_uart_install;
    }

    if (uart_param_config(lc76g_gnss->uart_port, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "config uart parameter failed");
        goto err_uart_config;
    }

    if (uart_set_pin(lc76g_gnss->uart_port, UART_PIN_NO_CHANGE, config->uart.rx_pin, UART_PIN_NO_CHANGE,
                     UART_PIN_NO_CHANGE) != ESP_OK) {
        ESP_LOGE(TAG, "config uart gpio failed");
        goto err_uart_config;
    }

    /* Set pattern interrupt, used to detect the end of a line */
    uart_enable_pattern_det_baud_intr(lc76g_gnss->uart_port, '\n', 1, 9, 0, 0);
    /* Set pattern queue size */
    uart_pattern_queue_reset(lc76g_gnss->uart_port, config->uart.event_queue_size);
    uart_flush(lc76g_gnss->uart_port);
    /* Create Event loop */
    esp_event_loop_args_t loop_args = {
        .queue_size = NMEA_EVENT_LOOP_QUEUE_SIZE,
        .task_name = NULL
    };

    if (esp_event_loop_create(&loop_args, &lc76g_gnss->event_loop_hdl) != ESP_OK) {
        ESP_LOGE(TAG, "create event loop faild");
        goto err_eloop;
    }

    BaseType_t err = xTaskCreatePinnedToCore(lc76g_task, "LC76G TASK", 4096, lc76g_gnss, 2, &lc76g_gnss->tsk_hdl, 1);
    if (err != pdTRUE) {
        ESP_LOGE(TAG, "create NMEA Parser task failed");
        goto err_task_create;
    }
    ESP_LOGI(TAG, "NMEA Parser init OK");
    return lc76g_gnss;

    /*Error Handling*/
    err_task_create:
        esp_event_loop_delete(lc76g_gnss->event_loop_hdl);
    err_eloop:
    err_uart_install:
        uart_driver_delete(lc76g_gnss->uart_port);
    err_uart_config:
    err_buffer:
        free(lc76g_gnss->buffer);
        lc76g_gnss->buffer = NULL;
    err_gps:
        free(lc76g_gnss);
        lc76g_gnss = NULL;
        return NULL;
}

/**
 * @brief Deinit NMEA Parser Use Uart
 *
 * @param nmea_hdl handle of NMEA parser
 * @return esp_err_t ESP_OK on success, ESP_FAIL on error
 */
static esp_err_t nmea_parser_deinit_uart(nmea_parser_handle_t nmea_hdl)
{
    lc76g_gnss_t *lc76g_gnss = nmea_hdl;
    vTaskDelete(lc76g_gnss->tsk_hdl);
    esp_event_loop_delete(lc76g_gnss->event_loop_hdl);
    esp_err_t err = uart_driver_delete(lc76g_gnss->uart_port);
    free(lc76g_gnss->buffer);
    free(lc76g_gnss);
    return err;
}

/**
 * @brief Converter two continuous numeric character into a uint8_t number
 *
 * @param digit_char numeric character
 * @return uint8_t result of converting
 */
static inline uint8_t convert_two_digit2number(const char *digit_char)
{
    return 10 * (digit_char[0] - '0') + (digit_char[1] - '0');
}

/**
 * @brief Parse UTC time in GPS statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_utc_time(lc76g_gnss_t *lc76g_gnss)
{
    lc76g_gnss->parent.tim.hour = convert_two_digit2number(lc76g_gnss->item_str + 0);
    lc76g_gnss->parent.tim.minute = convert_two_digit2number(lc76g_gnss->item_str + 2);
    lc76g_gnss->parent.tim.second = convert_two_digit2number(lc76g_gnss->item_str + 4);
    if (lc76g_gnss->item_str[6] == '.') {
        uint16_t tmp = 0;
        uint8_t i = 7;
        while (lc76g_gnss->item_str[i]) {
            tmp = 10 * tmp + lc76g_gnss->item_str[i] - '0';
            i++;
        }
        lc76g_gnss->parent.tim.thousand = tmp;
    }
}

/**
 * @brief parse latitude or longitude
 *              format of latitude in NMEA is ddmm.sss and longitude is dddmm.sss
 * @param lc76g_gnss lc76g_gnss_t type object
 * @return float Latitude or Longitude value (unit: degree)
 */
static float parse_lat_long(lc76g_gnss_t *lc76g_gnss)
{
    float ll = strtof(lc76g_gnss->item_str, NULL);
    int deg = ((int)ll) / 100;
    float min = ll - (deg * 100);
    ll = deg + min / 60.0f;
    return ll;
}

#if NMEA_STATEMENT_GGA
/**
 * @brief Parse GGA statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_gga(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GGA statement */
    switch (lc76g_gnss->item_num) {
    case 1: /* Process UTC time */
        parse_utc_time(lc76g_gnss);
        break;
    case 2: /* Latitude */
        lc76g_gnss->parent.latitude = parse_lat_long(lc76g_gnss);
        break;
    case 3: /* Latitude north(1)/south(-1) information */
        if (lc76g_gnss->item_str[0] == 'S' || lc76g_gnss->item_str[0] == 's') {
            lc76g_gnss->parent.latitude *= -1;
        }
        break;
    case 4: /* Longitude */
        lc76g_gnss->parent.longitude = parse_lat_long(lc76g_gnss);
        break;
    case 5: /* Longitude east(1)/west(-1) information */
        if (lc76g_gnss->item_str[0] == 'W' || lc76g_gnss->item_str[0] == 'w') {
            lc76g_gnss->parent.longitude *= -1;
        }
        break;
    case 6: /* Fix status */
        lc76g_gnss->parent.fix = (gps_fix_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    case 7: /* Satellites in use */
        lc76g_gnss->parent.sats_in_use = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    case 8: /* HDOP */
        lc76g_gnss->parent.dop_h = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 9: /* Altitude */
        lc76g_gnss->parent.altitude = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 11: /* Altitude above ellipsoid */
        lc76g_gnss->parent.altitude += strtof(lc76g_gnss->item_str, NULL);
        break;
    default:
        break;
    }
}
#endif

#if NMEA_STATEMENT_GSA
/**
 * @brief Parse GSA statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_gsa(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GSA statement */
    switch (lc76g_gnss->item_num) {
    case 2: /* Process fix mode */
        lc76g_gnss->parent.fix_mode = (gps_fix_mode_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    case 15: /* Process PDOP */
        lc76g_gnss->parent.dop_p = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 16: /* Process HDOP */
        lc76g_gnss->parent.dop_h = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 17: /* Process VDOP */
        lc76g_gnss->parent.dop_v = strtof(lc76g_gnss->item_str, NULL);
        break;
    default:
        /* Parse satellite IDs */
        if (lc76g_gnss->item_num >= 3 && lc76g_gnss->item_num <= 14) {
            lc76g_gnss->parent.sats_id_in_use[lc76g_gnss->item_num - 3] = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 10);
        }
        break;
    }
}
#endif

#if NMEA_STATEMENT_GSV
/**
 * @brief Parse GSV statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_gsv(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GSV statement */
    switch (lc76g_gnss->item_num) {
    case 1: /* total GSV numbers */
        lc76g_gnss->sat_count = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    case 2: /* Current GSV statement number */
        lc76g_gnss->sat_num = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    case 3: /* Process satellites in view */
        lc76g_gnss->parent.sats_in_view = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 10);
        break;
    default:
        if (lc76g_gnss->item_num >= 4 && lc76g_gnss->item_num <= 19) {
            uint8_t item_num = lc76g_gnss->item_num - 4; /* Normalize item number from 4-19 to 0-15 */
            uint8_t index = 4 * (lc76g_gnss->sat_num - 1) + item_num / 4; /* Get array index */
            if (index < GPS_MAX_SATELLITES_IN_VIEW) {
                uint32_t value = strtol(lc76g_gnss->item_str, NULL, 10);
                switch (item_num % 4) {
                case 0:
                    lc76g_gnss->parent.sats_desc_in_view[index].num = (uint8_t)value;
                    break;
                case 1:
                    lc76g_gnss->parent.sats_desc_in_view[index].elevation = (uint8_t)value;
                    break;
                case 2:
                    lc76g_gnss->parent.sats_desc_in_view[index].azimuth = (uint16_t)value;
                    break;
                case 3:
                    lc76g_gnss->parent.sats_desc_in_view[index].snr = (uint8_t)value;
                    break;
                default:
                    break;
                }
            }
        }
        break;
    }
}
#endif

#if NMEA_STATEMENT_RMC
/**
 * @brief Parse RMC statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_rmc(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GPRMC statement */
    switch (lc76g_gnss->item_num) {
    case 1:/* Process UTC time */
        parse_utc_time(lc76g_gnss);
        break;
    case 2: /* Process valid status */
        lc76g_gnss->parent.valid = (lc76g_gnss->item_str[0] == 'A');
        break;
    case 3:/* Latitude */
        lc76g_gnss->parent.latitude = parse_lat_long(lc76g_gnss);
        break;
    case 4: /* Latitude north(1)/south(-1) information */
        if (lc76g_gnss->item_str[0] == 'S' || lc76g_gnss->item_str[0] == 's') {
            lc76g_gnss->parent.latitude *= -1;
        }
        break;
    case 5: /* Longitude */
        lc76g_gnss->parent.longitude = parse_lat_long(lc76g_gnss);
        break;
    case 6: /* Longitude east(1)/west(-1) information */
        if (lc76g_gnss->item_str[0] == 'W' || lc76g_gnss->item_str[0] == 'w') {
            lc76g_gnss->parent.longitude *= -1;
        }
        break;
    case 7: /* Process ground speed in unit m/s */
        lc76g_gnss->parent.speed = strtof(lc76g_gnss->item_str, NULL) * 1.852;
        break;
    case 8: /* Process true course over ground */
        lc76g_gnss->parent.cog = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 9: /* Process date */
        lc76g_gnss->parent.date.day = convert_two_digit2number(lc76g_gnss->item_str + 0);
        lc76g_gnss->parent.date.month = convert_two_digit2number(lc76g_gnss->item_str + 2);
        lc76g_gnss->parent.date.year = convert_two_digit2number(lc76g_gnss->item_str + 4);
        break;
    case 10: /* Process magnetic variation */
        lc76g_gnss->parent.variation = strtof(lc76g_gnss->item_str, NULL);
        break;
    default:
        break;
    }
}
#endif

#if NMEA_STATEMENT_GLL
/**
 * @brief Parse GLL statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_gll(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GPGLL statement */
    switch (lc76g_gnss->item_num) {
    case 1:/* Latitude */
        lc76g_gnss->parent.latitude = parse_lat_long(lc76g_gnss);
        break;
    case 2: /* Latitude north(1)/south(-1) information */
        if (lc76g_gnss->item_str[0] == 'S' || lc76g_gnss->item_str[0] == 's') {
            lc76g_gnss->parent.latitude *= -1;
        }
        break;
    case 3: /* Longitude */
        lc76g_gnss->parent.longitude = parse_lat_long(lc76g_gnss);
        break;
    case 4: /* Longitude east(1)/west(-1) information */
        if (lc76g_gnss->item_str[0] == 'W' || lc76g_gnss->item_str[0] == 'w') {
            lc76g_gnss->parent.longitude *= -1;
        }
        break;
    case 5:/* Process UTC time */
        parse_utc_time(lc76g_gnss);
        break;
    case 6: /* Process valid status */
        lc76g_gnss->parent.valid = (lc76g_gnss->item_str[0] == 'A');
        break;
    default:
        break;
    }
}
#endif

#if NMEA_STATEMENT_VTG
/**
 * @brief Parse VTG statements
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void parse_vtg(lc76g_gnss_t *lc76g_gnss)
{
    /* Process GPVGT statement */
    switch (lc76g_gnss->item_num) {
    case 1: /* Process true course over ground */
        lc76g_gnss->parent.cog = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 3:/* Process magnetic variation */
        lc76g_gnss->parent.variation = strtof(lc76g_gnss->item_str, NULL);
        break;
    case 5:/* Process ground speed in unit m/s */
        lc76g_gnss->parent.speed = strtof(lc76g_gnss->item_str, NULL) * 1.852;//knots to m/s
        break;
    case 7:/* Process ground speed in unit m/s */
        lc76g_gnss->parent.speed = strtof(lc76g_gnss->item_str, NULL) / 3.6;//km/h to m/s
        break;
    default:
        break;
    }
}
#endif

/**
 * @brief Parse received item
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 * @return esp_err_t ESP_OK on success, ESP_FAIL on error
 */
static esp_err_t parse_item(lc76g_gnss_t *lc76g_gnss)
{
    esp_err_t err = ESP_OK;
    /* start of a statement */
    if (lc76g_gnss->item_num == 0 && lc76g_gnss->item_str[0] == '$') {
        if (0) {
        }
#if NMEA_STATEMENT_GGA
        else if (strstr(lc76g_gnss->item_str, "GGA")) {
            lc76g_gnss->cur_statement = STATEMENT_GGA;
        }
#endif
#if NMEA_STATEMENT_GSA
        else if (strstr(lc76g_gnss->item_str, "GSA")) {
            lc76g_gnss->cur_statement = STATEMENT_GSA;
        }
#endif
#if NMEA_STATEMENT_RMC
        else if (strstr(lc76g_gnss->item_str, "RMC")) {
            lc76g_gnss->cur_statement = STATEMENT_RMC;
        }
#endif
#if NMEA_STATEMENT_GSV
        else if (strstr(lc76g_gnss->item_str, "GSV")) {
            lc76g_gnss->cur_statement = STATEMENT_GSV;
        }
#endif
#if NMEA_STATEMENT_GLL
        else if (strstr(lc76g_gnss->item_str, "GLL")) {
            lc76g_gnss->cur_statement = STATEMENT_GLL;
        }
#endif
#if NMEA_STATEMENT_VTG
        else if (strstr(lc76g_gnss->item_str, "VTG")) {
            lc76g_gnss->cur_statement = STATEMENT_VTG;
        }
#endif
        else {
            lc76g_gnss->cur_statement = STATEMENT_UNKNOWN;
        }
        goto out;
    }
    /* Parse each item, depend on the type of the statement */
    if (lc76g_gnss->cur_statement == STATEMENT_UNKNOWN) {
        goto out;
    }
#if NMEA_STATEMENT_GGA
    else if (lc76g_gnss->cur_statement == STATEMENT_GGA) {
        parse_gga(lc76g_gnss);
    }
#endif
#if NMEA_STATEMENT_GSA
    else if (lc76g_gnss->cur_statement == STATEMENT_GSA) {
        parse_gsa(lc76g_gnss);
    }
#endif
#if NMEA_STATEMENT_GSV
    else if (lc76g_gnss->cur_statement == STATEMENT_GSV) {
        parse_gsv(lc76g_gnss);
    }
#endif
#if NMEA_STATEMENT_RMC
    else if (lc76g_gnss->cur_statement == STATEMENT_RMC) {
        parse_rmc(lc76g_gnss);
    }
#endif
#if NMEA_STATEMENT_GLL
    else if (lc76g_gnss->cur_statement == STATEMENT_GLL) {
        parse_gll(lc76g_gnss);
    }
#endif
#if NMEA_STATEMENT_VTG
    else if (lc76g_gnss->cur_statement == STATEMENT_VTG) {
        parse_vtg(lc76g_gnss);
    }
#endif
    else {
        err =  ESP_FAIL;
    }
out:
    return err;
}

/**
 * @brief Parse NMEA statements from GPS receiver
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 * @param len number of bytes to decode
 * @return esp_err_t ESP_OK on success, ESP_FAIL on error
 */
static esp_err_t gps_decode(lc76g_gnss_t *lc76g_gnss, size_t len)
{
    const uint8_t *d = lc76g_gnss->buffer;
    while (*d) {
        /* Start of a statement */
        if (*d == '$') {
            /* Reset runtime information */
            lc76g_gnss->asterisk = 0;
            lc76g_gnss->item_num = 0;
            lc76g_gnss->item_pos = 0;
            lc76g_gnss->cur_statement = 0;
            lc76g_gnss->crc = 0;
            lc76g_gnss->sat_count = 0;
            lc76g_gnss->sat_num = 0;
            /* Add character to item */
            lc76g_gnss->item_str[lc76g_gnss->item_pos++] = *d;
            lc76g_gnss->item_str[lc76g_gnss->item_pos] = '\0';
        }
        /* Detect item separator character */
        else if (*d == ',') {
            /* Parse current item */
            parse_item(lc76g_gnss);
            /* Add character to CRC computation */
            lc76g_gnss->crc ^= (uint8_t)(*d);
            /* Start with next item */
            lc76g_gnss->item_pos = 0;
            lc76g_gnss->item_str[0] = '\0';
            lc76g_gnss->item_num++;
        }
        /* End of CRC computation */
        else if (*d == '*') {
            /* Parse current item */
            parse_item(lc76g_gnss);
            /* Asterisk detected */
            lc76g_gnss->asterisk = 1;
            /* Start with next item */
            lc76g_gnss->item_pos = 0;
            lc76g_gnss->item_str[0] = '\0';
            lc76g_gnss->item_num++;
        }
        /* End of statement */
        else if (*d == '\r') {
            /* Convert received CRC from string (hex) to number */
            uint8_t crc = (uint8_t)strtol(lc76g_gnss->item_str, NULL, 16);
            /* CRC passed */
            if (lc76g_gnss->crc == crc) {
                switch (lc76g_gnss->cur_statement) {
#if NMEA_STATEMENT_GGA
                case STATEMENT_GGA:
                    lc76g_gnss->parsed_statement |= 1 << STATEMENT_GGA;
                    break;
#endif
#if NMEA_STATEMENT_GSA
                case STATEMENT_GSA:
                    lc76g_gnss->parsed_statement |= 1 << STATEMENT_GSA;
                    break;
#endif
#if NMEA_STATEMENT_RMC
                case STATEMENT_RMC:
                    lc76g_gnss->parsed_statement |= 1 << STATEMENT_RMC;
                    break;
#endif
#if NMEA_STATEMENT_GSV
                case STATEMENT_GSV:
                    if (lc76g_gnss->sat_num == lc76g_gnss->sat_count) {
                        lc76g_gnss->parsed_statement |= 1 << STATEMENT_GSV;
                    }
                    break;
#endif
#if NMEA_STATEMENT_GLL
                case STATEMENT_GLL:
                    lc76g_gnss->parsed_statement |= 1 << STATEMENT_GLL;
                    break;
#endif
#if NMEA_STATEMENT_VTG
                case STATEMENT_VTG:
                    lc76g_gnss->parsed_statement |= 1 << STATEMENT_VTG;
                    break;
#endif
                default:
                    break;
                }
                /* Check if all statements have been parsed */
                if (((lc76g_gnss->parsed_statement) & lc76g_gnss->all_statements) == lc76g_gnss->all_statements) {
                    lc76g_gnss->parsed_statement = 0;
                    /* Send signal to notify that GPS information has been updated */
                    esp_event_post_to(lc76g_gnss->event_loop_hdl, ESP_NMEA_EVENT, GPS_UPDATE,
                                      &(lc76g_gnss->parent), sizeof(gps_t), 100 / portTICK_PERIOD_MS);
                }
            } else {
                ESP_LOGD(TAG, "CRC Error for statement:%s", (char*)lc76g_gnss->buffer);
            }
            if (lc76g_gnss->cur_statement == STATEMENT_UNKNOWN) {
                /* Send signal to notify that one unknown statement has been met */
                esp_event_post_to(lc76g_gnss->event_loop_hdl, ESP_NMEA_EVENT, GPS_UNKNOWN,
                                  lc76g_gnss->buffer, len, 100 / portTICK_PERIOD_MS);
            }
        }
        /* Other non-space character */
        else {
            if (!(lc76g_gnss->asterisk)) {
                /* Add to CRC */
                lc76g_gnss->crc ^= (uint8_t)(*d);
            }
            /* Add character to item */
            lc76g_gnss->item_str[lc76g_gnss->item_pos++] = *d;
            lc76g_gnss->item_str[lc76g_gnss->item_pos] = '\0';
        }
        /* Process next character */
        d++;
    }
    return ESP_OK;
}

/**
 * @brief Handle when a pattern has been detected by uart
 *
 * @param lc76g_gnss lc76g_gnss_t type object
 */
static void esp_handle_uart_pattern(lc76g_gnss_t *lc76g_gnss)
{
    int pos = uart_pattern_pop_pos(lc76g_gnss->uart_port);
    if (pos != -1) {
        /* read one line(include '\n') */
        int read_len = uart_read_bytes(lc76g_gnss->uart_port, lc76g_gnss->buffer, pos + 1, 100 / portTICK_PERIOD_MS);
        /* make sure the line is a standard string */
        lc76g_gnss->buffer[read_len] = '\0';
        /* Send new line to handle */
        gps_decode(lc76g_gnss, read_len + 1);
    } else {
        ESP_LOGW(TAG, "Pattern Queue Size too small");
        uart_flush(lc76g_gnss->uart_port);
    }
}

/**
 * @brief Add user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @param handler_args handler specific arguments
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for the handler
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
static esp_err_t nmea_parser_add_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler, void *handler_args)
{
    lc76g_gnss_t *lc76g_gnss = nmea_hdl;
    return esp_event_handler_register_with(lc76g_gnss->event_loop_hdl, ESP_NMEA_EVENT, ESP_EVENT_ANY_ID,
                                           event_handler, handler_args);
}

/**
 * @brief Remove user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
static esp_err_t nmea_parser_remove_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler)
{
    lc76g_gnss_t *lc76g_gnss = nmea_hdl;
    return esp_event_handler_unregister_with(lc76g_gnss->event_loop_hdl, ESP_NMEA_EVENT, ESP_EVENT_ANY_ID, event_handler);
}

static void lc76g_task(void *arg)
{
    lc76g_gnss_t *lc76g_gnss = arg;
    uart_event_t event;

    for (;;) {
        if (xQueueReceive(lc76g_gnss->event_queue, &event, pdMS_TO_TICKS(200))) {
            switch (event.type) {
            case UART_DATA:
                break;
            case UART_FIFO_OVF:
                ESP_LOGW(TAG, "HW FIFO Overflow");
                uart_flush(lc76g_gnss->uart_port);
                xQueueReset(lc76g_gnss->event_queue);
                break;
            case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "Ring Buffer Full");
                uart_flush(lc76g_gnss->uart_port);
                xQueueReset(lc76g_gnss->event_queue);
                break;
            case UART_BREAK:
                ESP_LOGW(TAG, "Rx Break");
                break;
            case UART_PARITY_ERR:
                ESP_LOGE(TAG, "Parity Error");
                break;
            case UART_FRAME_ERR:
                ESP_LOGE(TAG, "Frame Error");
                break;
            case UART_PATTERN_DET:
                esp_handle_uart_pattern(lc76g_gnss);
                break;
            default:
                ESP_LOGW(TAG, "unknown uart event type: %d", event.type);
                break;
            }
        }
        /* Drive the event loop */
        esp_event_loop_run(lc76g_gnss->event_loop_hdl, pdMS_TO_TICKS(50));
    }
}
