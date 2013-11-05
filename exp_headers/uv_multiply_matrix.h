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

#ifndef UV_MULTIPLY_MATRIX_H
#define UV_MULTIPLY_MATRIX_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs Multiplies two matrices, both unsigned 8bit. Useful for doing trasformations. Example colour conversion  \n\n
  * Limitations and things to know \n
  *   - No of rows of transposed input image should be multiple of 16
  *   - Input matrix, mat1, should be in transposed form.
  *   - Transformation mat2, should be in normal(non transposed) form.
  *   - Output matrix will also be in transposed form.
  *   - This API currently does not do saturation if there is an overflow in the accumulator. This feature can be added on request.
  *   - Currently we handle unsigned data. we can support signed data as on request.
  *
  * @param mat1       		: Source matrix pointer, 8bit unsigned (data should be in transposed form)
  * @param mat2      		: Source matrix pointer, 8bit unsigned. Transformation matrix.
  * @param mat_out     		: Result matrix pointer, 16bit unsigned (data in transposed form)
  * @param shift     		: Acumulated result will be right shifted by this value.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_multiply_matrix(uv_handle sdk_handle, uv_mat *mat1, uv_mat *mat2, uv_mat16 *mat_out, int shift);


#endif /* UV_MULTIPLY_MATRIX_H */
