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

#ifndef UV_TABLE_LOOKUP_REF_H
#define UV_TABLE_LOOKUP_REF_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs table-lookup on source data with a table of size 32bytes
  *
  * @param source    		: Pointer to the data to be looked up
  * @param dest 	    	: Pointer to the looked up result
  * @param table_data    	: Pointer to the table data (32 bytes)
  * @param src_length    	: Length of the data to be looked up
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_table_lookup32_ref(uv_handle sdk_handle, char *source, char *dest, char *table_data, int src_length);

/** Performs table-lookup on source data with a table of size 64bytes
  *
  * @param source    		: Pointer to the data to be looked up
  * @param dest 	    	: Pointer to the looked up result
  * @param table_data    	: Pointer to the table data (64 bytes)
  * @param src_length    	: Length of the data to be looked up
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_table_lookup64_ref(uv_handle sdk_handle, char *source, char *dest, char *table_data, int src_length);

/** Performs table-lookup on source data with a table of size 128bytes
  *
  * @param source    		: Pointer to the data to be looked up
  * @param dest 	    	: Pointer to the looked up result
  * @param table_data    	: Pointer to the table data (128 bytes)
  * @param src_length    	: Length of the data to be looked up
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_table_lookup128_ref(uv_handle sdk_handle, char *source, char *dest, char *table_data, int src_length);

/** Performs table-lookup on source data with a table of size n-bytes
  *
  * @param source    		: Pointer to the data to be looked up
  * @param dest 	    	: Pointer to the looked up result
  * @param table_data    	: Pointer to the table data
  * @param src_length    	: Length of the data to be looked up
  * @param table_length     : Length of the Table data
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_table_lookup_ref(uv_handle sdk_handle, char *source, char *dest, char *table_data, int src_length,
                             int table_length);


#endif /* UV_TABLE_LOOKUP_REF_H */
