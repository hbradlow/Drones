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

#ifndef UV_NON_MAXIMUM_SUPPRESSION_REF_H
#define UV_NON_MAXIMUM_SUPPRESSION_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Non-maximum suppression and hysteresis thresholding on an edge image
  *
  * @param src_dx    		: Source absolute dx image pointer (from sobel output)
  * @param src_dy    		: Source absolute dy image pointer (from sobel output)
  * @param src_mag    		: Source magnitude image pointer (from sobel output)
  * @param src_sign    		: Source sign image pointer (from sobel output)
  * @param dst_img 		: Destination image pointer
  * @param low      		: Input low Threshold value
  * @param high      		: Input high Threshold value
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_non_maximum_suppression_ref(uv_handle sdk_handle, uv_image *src_dx, uv_image *src_dy, uv_image *src_mag,
                                        uv_image *src_sign, uv_image *dst_img,
                                        int low, int high);


#endif /* UV_NON_MAXIMUM_SUPPRESSION_REF_H */
