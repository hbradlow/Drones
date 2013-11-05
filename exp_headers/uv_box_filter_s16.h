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

#ifndef UV_BOX_FILTER_S16_H
#define UV_BOX_FILTER_S16_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs box filtering(of size 2x2) algorithm on an image(of datatype 16bit). Operation is equivalent to a convolution filter of kernel size 2x2 and kernel value all equal to 1.\n\n
  * Limitations\n
  *   - Width of the image should be a multiple of 8

  *
  * @param src_img 		: Source image pointer, unsigned 16-bit
  * @param dst_img 		: Destination image pointer, unsigned 16-bit
  * @param blocksize		: Redundant parameter. Will be removed.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_box_filter_s16(uv_handle handle, uv_image16 *src_img, uv_image16 *dst_img, int blocksize);


#endif /* UV_BOX_FILTER_S16_H */
