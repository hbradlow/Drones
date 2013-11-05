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

#ifndef UV_KMEANS3_H
#define UV_KMEANS3_H

#include <uv_errors.h>
#include <uv_types.h>


/** Performs k-means clustering image segmentation on an image of type 8bit unsigned, 3-channel\n\n
  * Limitations and things to know \n
  *   - Width of the image should be a multiple of 8.
  *   - Currently only supports k=3. But this can be extended on request.
  *   - Source image should be type of 8bit unsigned, (in our test examples we have chosen an 3-channel RGB input image)
  *   - Currently we are internally hardcoding the k-means centres to some specific values. We can make these input parameters, on request.
  *
  * @param src_img 		: Source image pointer, 8bit unsigned, 3-channel
  * @param dst_img	 	: Destination image pointer, 8bit unsigned. API assign 3 values for output. 60,120 or 180 depending on if the label is 1,2 or 3. This is for easily viewing the output image.
  * @param max_iterations	: Maximum number of iterations performed on clustering. It may exit earlier if the kmeans centres are not found to vary much from iteration to iteration.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_kmeans3(uv_handle sdk_handle, uv_image *src_img, uv_image *dst_img, int max_iterations);

#endif
