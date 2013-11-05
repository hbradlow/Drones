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

#ifndef UV_ADD_IMAGE16_H
#define UV_ADD_IMAGE16_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs addition on two images of datatype unsigned 16bit and produces and output of same datatype.\n\n
  * Limitations:\n
  *   - Width of the image should be a multiple of 8.
  *   - This API currently does not do saturation if there is an overflow. This feature can be added on request.
  *
  * @param src1_img 		: Source 1 image pointer
  * @param src2_img 		: Source 2 image pointer
  * @param dst_img 		: Destination image pointer of unsigned 16-bit
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_add_image16(uv_handle sdk_handle, uv_image16 *src1_img, uv_image16 *src2_img, uv_image16 *dst_img);


#endif /* UV_ADD_IMAGE16_H */
