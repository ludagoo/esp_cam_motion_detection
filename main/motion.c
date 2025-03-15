#include "motion.h"
#include "esp_log.h"

static const char* TAG = "motion";

// Simple motion detection by comparing pixel differences
bool motion_detect(camera_fb_t* fb1, camera_fb_t* fb2) {
    if (!fb1 || !fb2 || fb1->len != fb2->len) {
        ESP_LOGE(TAG, "Invalid frame buffers");
        return false;
    }

    uint32_t diff_sum = 0;
    uint32_t threshold = fb1->len / 10; // 10% of frame size as threshold

    for (size_t i = 0; i < fb1->len; i++) {
        int diff = abs((int)fb1->buf[i] - (int)fb2->buf[i]);
        diff_sum += diff;
    }

    ESP_LOGI(TAG, "Difference sum: %lu, Threshold: %lu", diff_sum, threshold);
    return diff_sum > threshold;
}