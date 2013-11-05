#ifndef UV_ORB_H
#define UV_ORB_H

#include "uv_types.h"

typedef struct Matcher
{

    int distance;

    int train_index;

    int query_index;

} uv_Matcher;

typedef struct OrbPatterns
{

    int kNumAngles; /// = 30

    ///** Contains the relative patterns (rotated ones in relative coordinates)
    int *relative_patterns_[360];         //changed from 30 to 360

    ///** Pattern loaded from the include files
    uv_mat32 rotated_patterns_[360];        //changed from 30 to 360

    unsigned int *intergral_5x5_diff;

} OrbPatterns;


/*!
 ORB implementation.
*/
typedef struct uv_ORB
{

    /** The size of the patch used when comparing regions in the patterns */
    int kKernelWidth;

///** Parameters tuning ORB */
///*CommonParams params_;

    /** Coefficient by which we divide the dimensions from one scale pyramid level to the next */
    float scale_factor_;
    /** The number of levels in the scale pyramid */
    unsigned int n_levels_;

    ///** The level at which the image is given
    ///* if 1, that means we will also look at the image scale_factor_ times bigger
    unsigned int first_level_;
    /** How far from the boundary the points should be */
    int edge_threshold_;

    /** The size of the patch that will be used for orientation and comparisons */
    int patch_size_;

    /** size of the half patch used for orientation computation, see Rosin - 1999 - Measuring Corner Properties */
    int half_patch_size_;

	OrbPatterns patterns[20]; ///** 20 max levels


    /** The number of desired features per scale */
    int *n_features_per_level_;

    float *scale_factor_per_level_;
    /** The overall number of desired features */
    int n_features_;

    /** the end of a row in a circular patch */
    int *u_max_;
    /** The patterns for each level (the patterns are the same, but not their offset */

} uv_ORB;

#endif

