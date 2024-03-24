#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"

static const char *TAG = "FATFS";
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
static const char *base_path = "/flash";

static void list_data_partitions(void);
static bool mount_fatfs(const char* partition_label);

esp_err_t flash_fatfs_init(void)
{
    const char *partition_label = "storage";

    list_data_partitions();
    if (!mount_fatfs(partition_label)) {
        ESP_LOGE(TAG, "Failed to mount FATFS");
        return ESP_FAIL;
    }

    uint64_t bytes_total, bytes_free;
    esp_vfs_fat_info(base_path, &bytes_total, &bytes_free);
    ESP_LOGI(TAG, "FAT FS: %lu kB total, %lu kB free", (uint32_t)bytes_total / 1024, (uint32_t)bytes_free / 1024);

    return ESP_OK;
}

static void list_data_partitions(void)
{
    ESP_LOGI(TAG, "Listing data partitions:");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

    for (; it != NULL; it = esp_partition_next(it)) {
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI(TAG, "- partition '%s', subtype %d, offset 0x%" PRIx32 ", size %" PRIu32 " kB",
        part->label, part->subtype, part->address, part->size / 1024);
    }

    esp_partition_iterator_release(it);
}

static bool mount_fatfs(const char* partition_label)
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
};
    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, partition_label, &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return false;
    }
    return true;
}