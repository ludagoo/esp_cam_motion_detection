#ifndef MOTION_H
#define MOTION_H

#include "esp_camera.h"

bool motion_detect(camera_fb_t* fb1, camera_fb_t* fb2);

#endif