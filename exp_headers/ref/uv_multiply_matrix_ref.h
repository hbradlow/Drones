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

#ifndef UV_MULTIPLY_MATRIX_REF_H
#define UV_MULTIPLY_MATRIX_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Multiplies two matrices
  *
  * @param mat1       		: First source matrix pointer
  * @param mat2      		: Second source matrix pointer
  * @param mat_out     		: Result matrix (depth 16-bits) pointer
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_multiply_matrix_ref(uv_handle sdk_handle, uv_mat *mat1, uv_mat *mat2, uv_mat16 *mat_out, int shift);


#endif /* UV_MULTIPLY_MATRIX_REF_H */
