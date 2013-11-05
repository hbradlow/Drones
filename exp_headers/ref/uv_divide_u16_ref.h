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

#ifndef UV_DIVIDE_U16_REF_H
#define UV_DIVIDE_U16_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs one-to-one division on two arrays
  *
  * @param source1    		: Pointer to the divident array
  * @param source2    		: Pointer to the diviser array
  * @param dest 	    	: Pointer to the result array
  * @param data_length    	: Length of array
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_divide_u16_ref(uv_handle sdk_handle, short *source1, short *source2,  short *result, int data_length);

#endif /* UV_DIVIDE_U16_REF_H */
