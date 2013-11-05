#include "orb.h"
#include "uv_types1.h"
#include <uv_errors.h>

//void uv_initialize_orb(uv_ORB *orb, uv_image *image, int max_keypoints, int n_level);
void uv_initialize_orb(uv_ORB *orb, uv_image *image, int max_keypoints, int n_level, float scale_fact);
void uv_free_orb(uv_ORB *orb);
uv_error uv_ORB_descriptor(uv_handle sdk_handle, uv_ORB *orb, int max_keypoints, int n_level, uv_image *image, uv_Keypoint *keypoints_in_out, uv_mat  *descriptors, int keypoint_type);

uv_error uv_ORB_matching(uv_handle sdk_handle, uv_mat *query_descriptors, uv_mat *train_descriptor, uv_Matcher matches[]);
