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

#ifndef UV_TYPES_H
#define UV_TYPES_H


#define UV_MAX_COLOUR_COMPS 4


typedef void* uv_handle;

typedef enum uv_colour_space
{
    UV_GRAY,
    UV_RGB32,
    UV_RGB65,
    UV_RGB24,
    UV_HSV,
    UV_RGB_PLANAR,
    UV_YUV_PLANAR
} uv_colour_space;


typedef struct uv_image_data_buffer
{
    /** Pointer to the base of 8-bit image data */
    unsigned char *pdata;
    /** Stride of image data*/
    int stride;
} uv_image_data_buffer;


typedef struct uv_image
{
    /** Arrary of #uv_image_data_buffer for every channel in an image*/
    uv_image_data_buffer channel_data[UV_MAX_COLOUR_COMPS];
    /** Width of the image in pixels */
    int width;
    /** Height of the image in pixels*/
    int height;
    /** Colour space of the image. see #uv_colour_space */
    uv_colour_space colourSpace;
} uv_image;

typedef struct uv_image16_data_buffer
{
    /** Pointer to the base of 16-bit image data */
    short *pdata;
    /** Stride of image data*/
    int stride;
} uv_image16_data_buffer;


typedef struct uv_image16
{
    /** Arrary of #uv_image_data_buffer for every channel in an image*/
    uv_image16_data_buffer channel_data[UV_MAX_COLOUR_COMPS];
    /** Width of the image in pixels */
    int width;
    /** Height of the image in pixels*/
    int height;
    /** Colour space of the image. see #uv_colour_space */
    uv_colour_space colourSpace;
} uv_image16;

typedef struct uv_image32_data_buffer
{
    /** Pointer to the base of 32-bit image data */
    int *pdata;
    /** Stride of image data*/
    int stride;
} uv_image32_data_buffer;

typedef struct uv_image32
{
    /** Arrary of #uv_image_data_buffer for every channel in an image*/
    uv_image32_data_buffer channel_data[UV_MAX_COLOUR_COMPS];
    /** Width of the image in pixels */
    int width;
    /** Height of the image in pixels*/
    int height;
    /** Colour space of the image. see #uv_colour_space */
    uv_colour_space colourSpace;
} uv_image32;

typedef struct uv_mat
{
    /** Pointer to the base 8-bit matrix data */
    char *mat_data;
    /** Number of rows of the matrix */
    int rows;
    /** Number of columns of the matrix */
    int cols;
} uv_mat;

typedef struct uv_mat16
{
    /** Pointer to the base 16-bit matrix data */
    short *mat_data;
    /** Number of rows of the matrix */
    int rows;
    /** Number of columns of the matrix */
    int cols;
} uv_mat16;

typedef struct uv_mat32
{
    /** Pointer to the base 32-bit matrix data */
    int *mat_data;
    /** Number of rows of the matrix */
    int rows;
    /** Number of columns of the matrix */
    int cols;
} uv_mat32;

typedef struct uv_matf32
{
    /** Pointer to the base 32-bit matrix data */
    float *mat_data;
    /** Number of rows of the matrix */
    int rows;
    /** Number of columns of the matrix */
    int cols;
} uv_matf32;

typedef struct uv_mat64
{
    /** Pointer to the base 64-bit matrix data */
    double *mat_data;
    /** Number of rows of the matrix */
    int rows;
    /** Number of columns of the matrix */
    int cols;
} uv_mat64;

#define uv_rect UvRect
typedef struct uv_rect
{
    /** x position of left top corner of the rectangle */
    int x;
    /** y position of left top corner of the rectangle */
    int y;
    /** Width of the rectangle */
    int width;
    /** Height of the rectangle */
    int height;
} uv_rect;
static uv_rect uvRect( int x, int y, int width, int height )
{
    uv_rect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}
//uv_rect  uvRect( int x, int y, int width, int height );

#endif /* UV_TYPES_H */
