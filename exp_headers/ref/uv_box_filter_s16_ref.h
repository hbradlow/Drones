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

#ifndef UV_BOX_FILTER_S16_REF_H
#define UV_BOX_FILTER_S16_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs box filtering algorithm on an image
  *
  * @param src_img 		: Source image pointer
  * @param dst_img 		: Destination image pointer
  * @param blocksize		: Block size of the kernel
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_box_filter_s16_ref(uv_handle handle, uv_image16 *src_img, uv_image16 *dst_img, int blocksize);


#endif /* UV_BOX_FILTER_S16_REF_H */
