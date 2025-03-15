#ifndef STORAGE_H
#define STORAGE_H

#include "esp_err.h"
#include "esp_camera.h"

esp_err_t storage_init(void);
esp_err_t storage_save_frame(camera_fb_t* fb, const char* filename);

#endif