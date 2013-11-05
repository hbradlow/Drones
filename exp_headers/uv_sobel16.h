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

#ifndef UV_SOBEL_H
#define UV_SOBEL_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Sobel edge detection on an image of type 8bit unsigned. Kernel used 3x3.\n\n
  * Limitations and things to know \n
  *   - Width of the image should be a multiple of 8.
  *
  * @param src_img 		: Source image pointer, 8bit unsigned
  * @param dx_img 		: Destination dx image pointer, 8bit unsigned.Absolute value of dx.
  * @param dy_img 		: Destination dy image pointer, 8bit unsigned.Absolute value of dy.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_sobel_16(uv_handle sdk_handle, uv_image *src_img, uv_image16 *dx_img, uv_image16 *dy_img);


#endif /* UV_SOBEL_H */
