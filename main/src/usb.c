#include <errno.h>
#include <dirent.h>
#include "esp_console.h"
#include "esp_partition.h"
#include "driver/gpio.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "tusb_console.h"
#include "tusb_msc_storage.h"
#include "tusb_tasks.h"

static const char *TAG = "TinyUSB";

// static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

/* TinyUSB descriptors
   ********************************************************************* */

/* Interfaces enumeration */
enum {
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,

    ITF_NUM_MSC,

    ITF_NUM_TOTAL
};

enum {
    TUSB_DESC_TOTAL_LEN = TUD_CONFIG_DESC_LEN +
                          CFG_TUD_CDC * TUD_CDC_DESC_LEN +
                          CFG_TUD_MSC * TUD_MSC_DESC_LEN
};

/* USB Endpoint numbers */
enum {
    EP_EMPTY = 0,

    EPNUM_0_CDC_NOTIF,
    EPNUM_0_CDC,

    EPNUM_MSC,
};

/* STRID */
enum {
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
    STRID_CDC_INTERFACE,
    STRID_MSC_INTERFACE,
};

static uint8_t const desc_configuration[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, STRID_CDC_INTERFACE, 0x80 | EPNUM_0_CDC_NOTIF, 8, EPNUM_0_CDC, 0x80 | EPNUM_0_CDC,
                       CFG_TUD_CDC_EP_BUFSIZE),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, STRID_MSC_INTERFACE, EPNUM_MSC, 0x80 | EPNUM_MSC, CFG_TUD_CDC_EP_BUFSIZE),
};

#define _PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))
static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USB_ESPRESSIF_VID,
    .idProduct = (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1)),
    .bcdDevice = CONFIG_TINYUSB_DESC_BCD_DEVICE,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

static char const *string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },              // 0: is supported language is English (0x0409)
    CONFIG_TINYUSB_DESC_MANUFACTURER_STRING,    // 1: Manufacturer
    CONFIG_TINYUSB_DESC_PRODUCT_STRING,         // 2: Product
    CONFIG_TINYUSB_DESC_SERIAL_STRING,          // 3: Serials, should use chip ID
    CONFIG_TINYUSB_DESC_CDC_STRING,             // 4: CDC Interface
    CONFIG_TINYUSB_DESC_MSC_STRING,             // 5: MSC Interface
};
/*********************************************************************** TinyUSB descriptors*/

#define BASE_PATH "/flash" // base path to mount the partition

static esp_err_t storage_init_spiflash(wl_handle_t *wl_handle);
static void storage_mount_changed_cb(tinyusb_msc_event_t *event);
static void msc_mount(void);
// static void msc_unmount(void);
// static void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event);
// static void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event);

#include <fcntl.h>
#include "vfs_tinyusb.h"

esp_err_t fatfs_init(void)
{
    ESP_LOGI(TAG, "Initializing fatfs...");

    static wl_handle_t wl_handle = WL_INVALID_HANDLE;
    ESP_ERROR_CHECK(storage_init_spiflash(&wl_handle));

    const tinyusb_msc_spiflash_config_t config_spi = {
        .wl_handle = wl_handle,
        .callback_mount_changed = storage_mount_changed_cb,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_spiflash(&config_spi));
    ESP_ERROR_CHECK(tinyusb_msc_register_callback(TINYUSB_MSC_EVENT_MOUNT_CHANGED, storage_mount_changed_cb));

    msc_mount();

    return ESP_OK;
}

esp_err_t usb_init(void)
{
    ESP_LOGI(TAG, "USB Composite initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &descriptor_config,
        .string_descriptor = string_desc_arr,
        .string_descriptor_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),
        .external_phy = false,
        .configuration_descriptor = desc_configuration,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    // tinyusb_config_cdcacm_t acm_cfg = {
    //     .usb_dev = TINYUSB_USBDEV_0,
    //     .cdc_port = TINYUSB_CDC_ACM_0,
    //     .rx_unread_buf_sz = 64,
    //     .callback_rx = &tinyusb_cdc_rx_callback,
    //     .callback_rx_wanted_char = NULL,
    //     .callback_line_state_changed = NULL,
    //     .callback_line_coding_changed = NULL
    // };
    //
    // ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    // /* the second way to register a callback */
    // ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
    //                     TINYUSB_CDC_ACM_0,
    //                     CDC_EVENT_LINE_STATE_CHANGED,
    //                     &tinyusb_cdc_line_state_changed_callback));
    // ESP_LOGI(TAG, "USB Composite initialization DONE");
    //
    // esp_tusb_init_console(TINYUSB_CDC_ACM_0);
    //
    // /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    // esp_vfs_tusb_cdc_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    // /* Move the caret to the beginning of the next line on '\n' */
    // esp_vfs_tusb_cdc_set_tx_line_endings(ESP_LINE_ENDINGS_LF);

    return ESP_OK;
}

esp_err_t usb_deinit(void)
{
    ESP_LOGI(TAG, "USB Composite deinitialization");
    ESP_ERROR_CHECK(tinyusb_driver_uninstall());
    tusb_stop_task();
    ESP_LOGI(TAG, "USB Composite deinitialization DONE");

    return ESP_OK;
}

static esp_err_t storage_init_spiflash(wl_handle_t *wl_handle)
{
    ESP_LOGI(TAG, "Initializing wear levelling");

    const esp_partition_t *data_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL);
    if (data_partition == NULL) {
        ESP_LOGE(TAG, "Failed to find FATFS partition. Check the partition table.");
        return ESP_ERR_NOT_FOUND;
    }

    return wl_mount(data_partition, wl_handle);
}

/* callback that is delivered when storage is mounted/unmounted by application. */
static void storage_mount_changed_cb(tinyusb_msc_event_t *event)
{
    ESP_LOGI(TAG, "Storage mounted to application: %s", event->mount_changed_data.is_mounted ? "Yes" : "No");
}

/* mount the partition and show all the files in BASE_PATH */
static void msc_mount(void)
{
    ESP_LOGI(TAG, "Mount storage...");
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(BASE_PATH));

    /* List all the files in this directory */
    ESP_LOGI(TAG, "\nls command output:");
    struct dirent *d;
    DIR *dh = opendir(BASE_PATH);
    if (!dh) {
        if (errno == ENOENT) {
            /* If the directory is not found */
            ESP_LOGE(TAG, "Directory doesn't exist %s", BASE_PATH);
        } else {
            /* If the directory is not readable then throw error and exit */
            ESP_LOGE(TAG, "Unable to read directory %s", BASE_PATH);
        }
        return;
    }
    /* While the next entry is not readable we will print directory files */
    while ((d = readdir(dh)) != NULL) {
        printf("%s\n", d->d_name);
    }
}

// static void msc_unmount(void)
// {
//     if (tinyusb_msc_storage_in_use_by_usb_host()) {
//         ESP_LOGE(TAG, "storage is already exposed");
//         return;
//     }
//     ESP_LOGI(TAG, "Unmount storage...");
//     ESP_ERROR_CHECK(tinyusb_msc_storage_unmount());
// }

// static void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
// {
//     /* initialization */
//     size_t rx_size = 0;
//
//     /* read */
//     esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
//     if (ret == ESP_OK) {
//         ESP_LOGI(TAG, "Data from channel %d:", itf);
//         ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_INFO);
//     } else {
//         ESP_LOGE(TAG, "Read error");
//     }
//
//     /* write back */
//     tinyusb_cdcacm_write_queue(itf, buf, rx_size);
//     tinyusb_cdcacm_write_flush(itf, 0);
// }
//
// static void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
// {
//     int dtr = event->line_state_changed_data.dtr;
//     int rts = event->line_state_changed_data.rts;
//     ESP_LOGI(TAG, "Line state changed on channel %d: DTR:%d, RTS:%d", itf, dtr, rts);
// }