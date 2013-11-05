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

#ifndef UV_HOUGH_LINES_REF_H
#define UV_HOUGH_LINES_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Hough line detection algorithm on an image
  *
  * @param src_img 		: Source image pointer
  * @param threshold 		: Threshold value
  * @param lines		: The array of lines
  * @param lineMax		: Total number of lines
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_hough_lines_ref(uv_handle sdk_handle, uv_image *src_img, int threshold, uv_mat32 *lines, int *lineMax);
uv_error uv_HoughLinesProbabalistic_ref(uv_handle sdk_handle, uv_image* src_img, int threshold, int lineLength, int lineGap, uv_rect *lines, int *linesMax );


#endif /* UV_HOUGH_LINES_REF_H */
