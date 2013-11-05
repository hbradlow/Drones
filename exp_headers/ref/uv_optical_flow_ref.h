/*
 *=BEGIN LICENCE HEADER
 *
 * Chipmonk CONFIDENTIAL
 *
 * Copyright (C) Chipmonk (2010).  All Rights Reserved.
 * www.chipmonk.in
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Chipmonk and its suppliers if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to Chipmonk and its suppliers and may be covered by
 * Indian and Foreign Patents, patents in process, and are protected
 * by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Chipmonk.
 *
 * Disclaimer : Regarding this entire document or any
 * portion of it (including the pseudocode and code), the author
 * makes no guarantees and is not responsible for any damage resulting
 * from its use.
 *
 *=END LICENCE HEADER
 */

#ifndef UV_OPTICAL_FLOW_REF_H
#define UV_OPTICAL_FLOW_REF_H

#include <uv_errors.h>
#include <uv_types.h>
#include <uv_types1.h>

/** Performs one-to-one addition of two images of datatype unsigned byte. Output buffer is of datatype unsigned 16bit.\n\n
  * Limitations:\n
  *   - width of image should be multiple of 16
  *
  *
  * @param src1_img 		: Source 1 image pointer, unsigned 8-bit
  * @param src2_img 		: Source 2 image pointer, unsigned 8-bit
  * @param dst_img 		: Destination image pointer, unsigned 16-bit
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error opencv_CalcOpticalFlowPyrLK_ref(uv_handle sdk_handle, uv_image *imgA, uv_image *imgB, uv_image *pyrA, uv_image *pyrB, uv_Point32f *featuresA, uv_Point32f *featuresB, int count,uv_Size winSize, int level, char *status, float *error,uv_TermCriteria criteria, int flags );


#endif /* UV_OPTICAL_FLOW_H */
