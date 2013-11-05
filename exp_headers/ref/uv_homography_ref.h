#ifndef UV_HOMOGRAPHY_REF_H
#define UV_HOMOGRAPHY_REF_H

#include <uv_types1.h>
#include <uv_types.h>
#include <uv_errors.h>

uv_error uv_findHomography_ref( uv_handle sdk_handle, UvPoint2D16* points1, UvPoint2D16* points2,
                                uv_matf32* _H, double ransacReprojThreshold, int numberoffeatures );

#endif /* UV_HOMOGRAPHY_REF_H */
