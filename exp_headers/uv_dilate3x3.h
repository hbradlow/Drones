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

#ifndef UV_DILATE3X3_H
#define UV_DILATE3X3_H

#include <uv_errors.h>
#include <uv_types.h>
/** Performs dilate operation on a binary image.This function is hardcoded to optimized for kenel size 3X3 array. \n\n
  * Limitations and things to know \n
  *
  *   - Width of the image should be a multiple of 16
  *   - Generic version of dilate algorithm optimized for kernel size 3X3 array and is faster than common dilate function.
  *
  * @param src_img 		: Source image pointer, type 8 bit unsigned. Expects binary data(255 or 0).
  * @param dst_img 		: Destination image pointer, type 8 bit unsigned. Binary data(255 or 0).
  * @param kernel 		: Pointer to the structuring element array, 8-bit unsigned values. Expects binary values, either 1 or 0.
  *
  * @return #UV_E_SUCCESS if successful
  */
uv_error uv_dilate3x3(uv_handle sdk_handle, uv_image *src_img, uv_image *dst_img,
                      char *kernel);


#endif /* UV_DILATE3X3_H */