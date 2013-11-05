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

#ifndef UV_HARRIS_H
#define UV_HARRIS_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Harris corner detection algorithm on an image \n\n
  * Limitations and things to know \n
  *   - Width of the images should be a multiple of 4.
  *   - param buf Points to a temp buffer.This structure and the temp buffers that it points to is allocated and initialized inside #uv_init_harris_buf, before calling this API(#uv_harris). And then freed by calling  #uv_free_harris_buf.
  *   - Internally performs sobel function with kernel size 3x3.
  *   - This API will not return coordinates of harris corners. It will not do thresholding of image either.
  *   - This API will compute the following formula\n
  *       output = (dx2 * dy2) - (dxdy)*(dxdy) - 0.04 * (dx2 + dy2)*(dx2 + dy2)
  *
  * @param src_img 		: Source image pointer of type 8bit unsigned
  * @param dst_img 		: Destination image pointer of type 32bit unsigned
  * @param *buf			: Pointer to the temp buffer (should be initialized or freed externally)
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_harris(uv_handle sdk_handle, uv_image *src_img, uv_image32 *dst_img, unsigned char *buf);


#endif /* UV_HARRIS_H */
