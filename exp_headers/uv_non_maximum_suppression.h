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

#ifndef UV_NON_MAXIMUM_SUPPRESSION_H
#define UV_NON_MAXIMUM_SUPPRESSION_H

#include <uv_errors.h>
#include <uv_types.h>


/** Performs Non-maximum suppression and hysteresis thresholding on an edge image\n\n
  * Limitations and things to know \n
  *   - Width of the image should be a multiple of 16
  *   - param src_dx points to x-gradient from Sobel operator.
  *   - param src_dy points to y-gradient from Sobel operator.
  *   - param src_mag points to Gradient magnitude from Sobel operator.
  *
  * @param src_dx    		: Pointer to absolute value of dx(x-gradient from Sobel operator), 8bit unsigned
  * @param src_dy    		: Pointer to absolute value of dy(y-gradient from Sobel operator), 8bit unsigned
  * @param src_mag    		: Pointer to absolute value of magnitude(Gradient magnitude from Sobel operator), 8bit unsigned
  * @param src_sign    		: Source sign(Gradient sign from sobel operator) image pointer, 8bit unsigned. 1 indicates -ve. 0 indicate >=0. no other value is allowed.
  * @param dst_img 		: Destination image pointer, type 8 bit unsigned. Binary data(255 or 0).
  * @param low      		: Hysteresis threshold low
  * @param high      		: Hysteresis threshold high
  *
  * @return #UV_E_SUCCESS if successful
  */
uv_error uv_non_maximum_suppression(uv_handle sdk_handle, uv_image *src_dx, uv_image *src_dy, uv_image *src_mag,
                                    uv_image *src_sign, uv_image *dst_img,
                                    int low, int high);


#endif /* UV_NON_MAXIMUM_SUPPRESSION_H */
