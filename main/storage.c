#include "storage.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_log.h"
#include "esp_camera.h"

static const char* TAG = "storage";
static bool sd_mounted = false;

esp_err_t storage_init(void) {
    esp_err_t ret;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1; // ESP-CAM typically uses 1-bit mode

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t* card;
    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SD card mount failed: 0x%x", ret);
        return ret;
    }

    sd_mounted = true;
    ESP_LOGI(TAG, "SD card mounted");
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}

esp_err_t storage_save_frame(camera_fb_t* fb, const char* filename) {
    if (!sd_mounted) {
        ESP_LOGE(TAG, "SD card not mounted");
        return ESP_FAIL;
    }

    char path[32];
    snprintf(path, sizeof(path), "/sdcard/%s", filename);

    FILE* f = fopen(path, "wb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file %s", path);
        return ESP_FAIL;
    }

    size_t written = fwrite(fb->buf, 1, fb->len, f);
    fclose(f);

    if (written != fb->len) {
        ESP_LOGE(TAG, "Failed to write frame to %s", path);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Saved frame to %s", path);
    return ESP_OK;
}