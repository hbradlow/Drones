/*
 *=BEGIN LICENCE HEADER
 *
 * UNCANNY VISION CONFIDENTIAL
 *
 * Copyright (C) Uncanny Vision (2013).  All Rights Reserved.
 * http://www.uncannyvision.com
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Uncanny Vision and its suppliers if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to Uncanny Vision and its suppliers and may be covered by
 * Indian and Foreign Patents, patents in process, and are protected
 * by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Uncanny Vision.
 *
 * Disclaimer : Regarding this entire document or any
 * portion of it (including the pseudocode and code), the author
 * makes no guarantees and is not responsible for any damage resulting
 * from its use.
 *
 *=END LICENCE HEADER
 */

#ifndef UV_CANNY_H
#define UV_CANNY_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Canny edge detection algorithm on a grayscale image. \n\n
  * Limitations and things to know\n
  *   - Width of the images should be a multiple of 8.
  *   - This API does not do Gaussian smoothing. If required, it can be done outside this function, since its a pre-processing step.
  *   - The sobel operation inside this API uses a 3x3 kernel
  *   - Input is of type 8 bit gray image
  *   - Output is a 8-bit binary image( each byte is of either value 255, or 0)
  *
  *
  * @param src_img 		: Source image pointer, 8 bit unsigned
  * @param dst_img 		: Destination image pointer, 8 bit unsigned. Output is binary(255  or 0)
  * @param low       		: Low value for hysteresis threshold
  * @param high      		: High  value for hysteresis threshold
  * @param buf                  : pointer to the buffer allocated using #uv_init_canny_buf
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_canny(uv_handle sdk_handle, uv_image *src_img, uv_image *dst_img, int low, int high, unsigned char *buf);

#endif /* UV_CANNY_H */
