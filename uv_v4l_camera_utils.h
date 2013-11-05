#ifndef UV_V4L_CAMERA_UTILS_H
#define UV_V4L_CAMERA_UTILS_H

#include <uv_types.h>
#include <uv_errors.h>

uv_error uv_camera_init(char *dev_str, int w, int h, int pixelFormat);
uv_error uv_camera_capture(int greyWanted, uv_image *captured_img, int pixelFormat);
uv_error uv_camera_close(void);
void convertNV12toRGB(char *y, char *uv, char *rgb, int w, int h);

#endif
