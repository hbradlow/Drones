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

#ifndef UV_RESIZE_BILINEAR_DOWN_H
#define UV_RESIZE_BILINEAR_DOWN_H

#include <uv_errors.h>
#include <uv_types.h>

/** Downscales images. Uses bilinear interpolation for resize.\n\n
  * Limitations and things to know \n
  *   - width of output image, w2, should be a multiple of 20
  *   - Does not try to maintain aspect ratio. Can resizes vertically and horizontally at different ratios.
  *   - Internally it uses a resize ratio approximated to 32/x = output_width/input_width, where x is an integer.
  *
  * @param src_img		: Source image pointer
  * @param dst_img 		: Destination image pointer
  * @param w2			: Destination width, this value overrides the width of destination image inside dst_img. Will fix this.
  * @param h2			: Destination height, this value overrides the height of destination image inside dst_img. Will fix this.
  *
  * @return #UV_E_SUCCESS if successful
  */
uv_error uv_resize_bilinear_down(uv_handle sdk_handle, uv_image *src_img, uv_image *dst_img, int w2, int h2);

#endif /* UV_RESIZE_BILINEAR_DOWN_H */
