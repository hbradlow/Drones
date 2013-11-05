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

#ifndef UV_TRANSPOSE_H
#define UV_TRANSPOSE_H

#include <uv_errors.h>
#include <uv_types.h>

/** Transposes an image of type 8bit unsigned. Output is also of same datatype. \n\n
  * Limitations and things to know \n
  *   - width of image should be multiple of 8
  *   - Height of image should be multiple of 8
  *
  * @param src_img 		: Source image pointer of type 8bit unsigned.
  * @param dst_img 		: Destination image pointer of type 8bit unsigned.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_transpose(uv_handle sdk_handle, uv_image *src_img, uv_image *dst_img);


#endif /* UV_TRANSPOSE_H */
