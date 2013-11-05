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

#ifndef UV_DIVIDE_U16_H
#define UV_DIVIDE_U16_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs one-to-one division on two arrays containing unsigned 16bit values.\n\n
  * Limitations and things to know \n
  *   - width of image should be multiple of 8
  *   - This API internally uses a Neon instruction, which is fast but an approximation(according to ARM Technical manual). Hence one-bit errors are possible in quotient. However when the quotient is very large, the error is larger. For example the result of 32553/11 is actually equal to 2959. However this API will output 2956.
  *   - When divisor is equal to 0, then this API returns 0xFFFFFFFF
  *
  * @param source1    		: Pointer to the dividend array
  * @param source2    		: Pointer to the divisor array
  * @param dest 	    	: Pointer to the result array. Quotient. = *source1/*source2
  * @param data_length    	: Length of array. Number of divisions to be computed.
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_divide_u16(uv_handle sdk_handle, short *source1, short *source2,  short  *dest, int data_length);

#endif /* UV_DIVIDE_U16_H */
