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

#ifndef UV_GET_CONNECTEDCOMPS_REF_H
#define UV_GET_CONNECTEDCOMPS_REF_H

#include <uv_errors.h>
#include <uv_types.h>
/** Searches and finds connected components in a grayscale image
  *
  * @param src_img 		: Source image pointer
  * @param buf       		: Pointer to the buffer. This should be initialized/freed before/after function call using #uv_init_connectedcomps_buf/#uv_free_connectedcomps_buf
  * @param bound_box        : Pointer to the resulting bounding box array (must be allocated externally)
  * @param ncomps           : Number of connected components obtained
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_get_connectedcomps_ref(uv_handle sdk_handle, uv_image *src_img, unsigned char *buf, uv_rect *bound_box, int *ncomps);


#endif /* UV_GET_CONNECTEDCOMPS_REF_H */
