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

#ifndef UV_CONVOLUTION_16U_8U_7x1_H
#define UV_CONVOLUTION_16U_8U_7x1_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs convolution on an image of type 8bit unsigned. Output is also of same datatype.This function is hardcoded to  optimized for kenel size 7X1 array.  \n\n
 * Limitations and things to know \n
 *   - width of image should be multiple of 8
 *   - kernel is of datatype 8bit signed
 *   - kernel shift indicates the right shift to be done before writing out a pixel.
 *
 * @param src_img		: unsigned 8bit Source image pointer
 * @param dst_img 		: unsigned 8bit Destination image pointer
 * @param kernel 		: Pointer to signed 8-bit kernel data, with size 7X1 array
 * @param kernel_shift		: Kernel shift
 *
 * @return #UV_E_SUCCESS if successful
 */

uv_error uv_convolution_16u_8u_7x1(uv_handle sdk_handle, uv_image16 * src_img, uv_image * dst_img, char * kernel, int kernel_shift);


#endif /* UV_CONVOLUTION_8U_7x1 */
