#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "camera.h"
#include "motion.h"
#include "storage.h"

static const char* TAG = "app_main";

void app_main(void) {
    // Initialize components
    if (camera_init() != ESP_OK) {
        ESP_LOGE(TAG, "Camera initialization failed");
        return;
    }
    if (storage_init() != ESP_OK) {
        ESP_LOGE(TAG, "Storage initialization failed");
        return;
    }

    camera_fb_t* fb1 = NULL;
    camera_fb_t* fb2 = NULL;
    bool recording = false;
    int frame_count = 0;

    while (1) {
        // Capture two consecutive frames
        fb1 = fb2;
        fb2 = camera_capture();
        if (!fb2) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            continue;
        }

        if (fb1) {
            // Detect motion
            if (motion_detect(fb1, fb2)) {
                if (!recording) {
                    ESP_LOGI(TAG, "Motion detected, starting recording");
                    recording = true;
                }

                // Save frame
                char filename[32];
                snprintf(filename, sizeof(filename), "frame_%d.jpg", frame_count++);
                storage_save_frame(fb2, filename);
            } else if (recording) {
                ESP_LOGI(TAG, "Motion stopped, ending recording");
                recording = false;
            }

            // Free the previous frame
            esp_camera_fb_return(fb1);
        }

        // Delay to control frame rate
        vTaskDelay(100 / portTICK_PERIOD_MS); // ~10 FPS
    }
}