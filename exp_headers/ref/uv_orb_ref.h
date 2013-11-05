

#include <orb.h>
#include <uv_types1.h>
#include <uv_types.h>
#include <uv_errors.h>
//void initializing_orb(uv_ORB *obj, int max_keypoints, int n_level);
uv_error ORB_descriptor_ref(uv_handle sdk_handle, uv_ORB *orb, int max_keypoints, int n_level, uv_image *image, uv_Keypoint *keypoints_in_out, uv_mat  *descriptors, int flag);

uv_error uv_ORB_matching_ref(uv_handle sdk_handle, uv_mat *query_descriptors, uv_mat *train_descriptor, uv_Matcher matches[]);

