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

#ifndef UV_OPTICAL_FLOW_H
#define UV_OPTICAL_FLOW_H

#include <uv_errors.h>
#include <uv_types.h>
#include <uv_types1.h>

uv_error uv_optical_flow(uv_handle handle, uv_image *imgA, uv_image *imgB, uv_image *pyrA, uv_image *pyrB, uv_Point32f *featuresA,
					 uv_Point32f *featuresB, int count, char *status,float *error, int level,
					 uv_TermCriteria criteria );

uv_error uv_optical_flow_(uv_handle sdk_handle, uv_image *imgA, uv_image *imgB, uv_image *pyrA, uv_image *pyrB, uv_Point32f *featuresA,
                         uv_Point32f *featuresB, int count, char *status,float *error, int level,
                         uv_TermCriteria criteria );
#endif /* UV_OPTICAL_FLOW_H */
