#include "camera.h"
#include "esp_log.h"

static const char* TAG = "camera";

esp_err_t camera_init(void) {
    camera_config_t config = {
        .pin_pwdn = -1,           // ESP-CAM has no power-down pin
        .pin_reset = -1,          // ESP-CAM has no reset pin
        .pin_xclk = 0,
        .pin_sscb_sda = 26,
        .pin_sscb_scl = 27,
        .pin_d7 = 35,
        .pin_d6 = 34,
        .pin_d5 = 39,
        .pin_d4 = 36,
        .pin_d3 = 21,
        .pin_d2 = 19,
        .pin_d1 = 18,
        .pin_d0 = 5,
        .pin_vsync = 25,
        .pin_href = 23,
        .pin_pclk = 22,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA, // 640x480 for reasonable performance
        .jpeg_quality = 10,          // 0-63, lower is higher quality
        .fb_count = 2                // Double buffering
    };

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
    }
    return err;
}

camera_fb_t* camera_capture(void) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
    }
    return fb;
}