/**
 * @file lc76g.h
 * @brief lc76g初始化头文件。
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/uart.h"

#define GPS_MAX_SATELLITES_IN_USE (12)
#define GPS_MAX_SATELLITES_IN_VIEW (16)

#define NMEA_STATEMENT_GSA 1
#define NMEA_STATEMENT_GSV 1
#define NMEA_STATEMENT_GGA 1
#define NMEA_STATEMENT_RMC 1
#define NMEA_STATEMENT_GLL 1
#define NMEA_STATEMENT_VTG 1

#define NMEA_PARSER_CONFIG_DEFAULT()       \
    {                                      \
        .uart = {                          \
            .uart_port = UART_NUM_1,       \
            .rx_pin = 2,                   \
            .baud_rate = 115200,           \
            .data_bits = UART_DATA_8_BITS, \
            .parity = UART_PARITY_DISABLE, \
            .stop_bits = UART_STOP_BITS_1, \
            .event_queue_size = 32         \
        }                                  \
    }

typedef void *nmea_parser_handle_t;

typedef struct {
    struct {
        uart_port_t uart_port; /*!< UART port number */
        uint32_t rx_pin; /*!< UART Rx Pin number */
        uint32_t baud_rate; /*!< UART baud rate */
        uart_word_length_t data_bits; /*!< UART data bits length */
        uart_parity_t parity; /*!< UART parity */
        uart_stop_bits_t stop_bits; /*!< UART stop bits length */
        uint32_t event_queue_size; /*!< UART event queue size */
    } uart; /*!< UART specific configuration */
} nmea_parser_config_t;

typedef enum {
    GPS_UPDATE, /*!< GPS information has been updated */
    GPS_UNKNOWN /*!< Unknown statements detected */
} nmea_event_id_t;

typedef enum {
    GPS_FIX_INVALID, /*!< Not fixed */
    GPS_FIX_GPS,     /*!< GPS */
    GPS_FIX_DGPS,    /*!< Differential GPS */
} gps_fix_t;

typedef struct {
    uint8_t hour;      /*!< Hour */
    uint8_t minute;    /*!< Minute */
    uint8_t second;    /*!< Second */
    uint16_t thousand; /*!< Thousand */
} gps_time_t;

typedef enum {
    GPS_MODE_INVALID = 1, /*!< Not fixed */
    GPS_MODE_2D,          /*!< 2D GPS */
    GPS_MODE_3D           /*!< 3D GPS */
} gps_fix_mode_t;

typedef struct {
    uint8_t num;       /*!< Satellite number */
    uint8_t elevation; /*!< Satellite elevation */
    uint16_t azimuth;  /*!< Satellite azimuth */
    uint8_t snr;       /*!< Satellite signal noise ratio */
} gps_satellite_t;

typedef struct {
    uint8_t day;   /*!< Day (start from 1) */
    uint8_t month; /*!< Month (start from 1) */
    uint16_t year; /*!< Year (start from 2000) */
} gps_date_t;

typedef struct {
    float latitude;                                                /*!< Latitude (degrees) */
    float longitude;                                               /*!< Longitude (degrees) */
    float altitude;                                                /*!< Altitude (meters) */
    gps_fix_t fix;                                                 /*!< Fix status */
    uint8_t sats_in_use;                                           /*!< Number of satellites in use */
    gps_time_t tim;                                                /*!< time in UTC */
    gps_fix_mode_t fix_mode;                                       /*!< Fix mode */
    uint8_t sats_id_in_use[GPS_MAX_SATELLITES_IN_USE];             /*!< ID list of satellite in use */
    float dop_h;                                                   /*!< Horizontal dilution of precision */
    float dop_p;                                                   /*!< Position dilution of precision  */
    float dop_v;                                                   /*!< Vertical dilution of precision  */
    uint8_t sats_in_view;                                          /*!< Number of satellites in view */
    gps_satellite_t sats_desc_in_view[GPS_MAX_SATELLITES_IN_VIEW]; /*!< Information of satellites in view */
    gps_date_t date;                                               /*!< Fix date */
    bool valid;                                                    /*!< GPS validity */
    float speed;                                                   /*!< Ground speed, unit: m/s */
    float cog;                                                     /*!< Course over ground */
    float variation;                                               /*!< Magnetic variation */
} gps_t;

typedef enum {
    STATEMENT_UNKNOWN = 0, /*!< Unknown statement */
    STATEMENT_GGA,         /*!< GGA */
    STATEMENT_GSA,         /*!< GSA */
    STATEMENT_RMC,         /*!< RMC */
    STATEMENT_GSV,         /*!< GSV */
    STATEMENT_GLL,         /*!< GLL */
    STATEMENT_VTG          /*!< VTG */
} nmea_statement_t;

esp_err_t lc76g_init(void);
esp_err_t lc76g_deinit(void);
gps_t lc76g_get_data(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif