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


#ifndef UV_KMEANS_REF_H
#define UV_KMEANS_REF_H

#include <uv_errors.h>
#include <uv_types.h>


/** Performs k-means clustering on a 3-channel image
  *
  * @param src_img 		: Source image pointer
  * @param label_img	: Destination label image pointer
  * @param k			: Number of clusters
  * @param max_iterations	: Maximum number of iterations performed on clustering
  *
  * @return #UV_E_SUCCESS if successful
  */


uv_error uv_kmeans_ref (uv_handle sdk_handle, uv_image *src_img, uv_image *label_img, int k, int max_iterations);

#endif