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

#ifndef UV_FAST_REF_H
#define UV_FAST_REF_H

#include <uv_errors.h>
#include <uv_types.h>
#include <uv_types1.h>


uv_error uv_fast9Detect_ref(uv_handle sdk_handle, const uv_image *img, int threshold, uv_Keypoint ret_corners[], int *fast_corners, int boarder_size);

void uv_fast9ComputeScore_ref(const uv_image *img, int threshold, uv_Keypoint corners[], int fast_corners);
void fastNonmaxSuppression(const uv_Keypoint corners[] , int *tot_corners, uv_Keypoint ret_nonmax[]);

void fast9ComputeScores_opencv(const uv_image *img, int b, uv_Keypoint corners[], int fast_corners);


#endif /* UV_VISION_H  */
