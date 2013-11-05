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

#ifndef UV_IMAGE_UTILS_H
#define UV_IMAGE_UTILS_H

#include <uv_errors.h>
#include <uv_types.h>
/*
uv_SaveImage dumps the buffer into a binary file
uv_LoadImage reads the image binary file into an aligned buffer
*/

//unsigned char* uv_init_buf(int width, int height, int channels);

//void uv_free_buf(unsigned char *buf);

uv_error uv_create_image(int width, int height, uv_colour_space clr_spc,
						uv_image *im);

uv_error uv_create_image16(int width, int height, uv_colour_space clr_spc,
						uv_image16 *im);

uv_error uv_create_image32(int width, int height, uv_colour_space clr_spc,
						uv_image32 *im);

uv_error uv_release_image(uv_image* im);

uv_error uv_release_image16(uv_image16* im);

uv_error uv_release_image32(uv_image32* im);


uv_error uv_RandomImage( uv_image *im, const int width,
                        const int height, const int channel, const int depth );

uv_error uv_LoadImage(const char *filename, uv_image *image_out);

uv_error uv_SaveImage(char *filename, uv_image *im);
uv_error uv_SaveImage16(char *filename, uv_image16 *im);
uv_error uv_SaveImage32(char *filename, uv_image32 *im);

uv_error uv_SaveImage_bmp(char *filename, uv_image *im);

uv_error uv_SaveImage_pgm(char *filename, uv_image *im);

uv_error uv_create_matrix(int rows, int cols, uv_mat *mat);
uv_error uv_create_matrix16(int rows, int cols, uv_mat16 *mat);
uv_error uv_create_matrix32(int rows, int cols, uv_mat32 *mat);

uv_error uv_release_matrix(uv_mat *mat);
uv_error uv_release_matrix16(uv_mat16 *mat);
uv_error uv_release_matrix32(uv_mat32 *mat);
#endif /* UV_IMAGE_UTILS_H */
