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

#ifndef UV_CUMULATIVE_HISTOGRAM_32BINS_H
#define UV_CUMULATIVE_HISTOGRAM_32BINS_H

#include <uv_errors.h>
#include <uv_types.h>

/** Performs 32-bin Cumulative histogram on an image.\n\n
  * Limitations and things to know \n
  *
  *   - width of image should be multiple of 8
  *   - Returns cumulative histogram of 32 bins.
  *   - The parameter bins is actually the bin edges that can be defined by the user.
  *   - The histogram array hist contains the following values. hist[n] contains the number of pixels, whose value <= bin[n].
  *   - By finding difference of each histogram bin(hist[n-1]) with its neighbouring bin(hist[n]) will give the number of pixels lying between the 2 bins, bin[n-1] and bin[n].
  *
  * @param src_img 		: Source image pointer, datatype 8bit unsigned
  * @param hist      		: Output buffer. Pointer to the histogram data, datatype 64bit unsigned
  * @param bins      		: Input param. Pointer to the bins data, datatype 8bit unsigned. Indicates the edges of the bins, that can be defined by the user. The histogram array hist contains the following values. hist[n] contains the number of pixels <= bin[n].
  *
  * @return #UV_E_SUCCESS if successful
  */

uv_error uv_cumulative_histogram_32bins(uv_handle sdk_handle, uv_image *src_img,
                                            unsigned long *hist,
                                            unsigned char *bins);


#endif /* UV_CUMULATIVE_HISTOGRAM_32BINS_H */
