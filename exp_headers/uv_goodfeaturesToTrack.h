#include <uv_types1.h>
#include <uv_types.h>

uv_error
uv_goodfeaturesToTrack(uv_handle sdk_handle, uv_image *input_image, uv_image32 *harris_temp, 
			uv_Point32f *features, int *number_of_features,
                       	float *quality_level, int *min_distance );


