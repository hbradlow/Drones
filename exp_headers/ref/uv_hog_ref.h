#ifndef UV_HOG_REF_H
#define UV_HOG_REF_H

#include <stdbool.h>
#include <uv_types.h>
#include <uv_types1.h>

void uv_hogDetectSingleScale_ref( const uv_image* uvImg, uv_Point* hits, int* foundObjectCount, double hitThreshold,
                            UvSize winStride, UvSize padding);

void uv_hogDetectMultiScale_ref( uv_handle _handle, const uv_image* uvImg, uv_rect* foundLocations, int* foundObjectCount,
                           double hitThreshold, UvSize winStride, UvSize padding,
                           double scale0, double finalThreshold, bool useMeanshiftGrouping);

#endif
