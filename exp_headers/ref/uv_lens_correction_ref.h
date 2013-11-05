
#ifndef HOMO_GRAPHIC_REF
#define HOMO_GRAPHIC_REF


#include <uv_errors.h>
#include <uv_types.h>


uv_error uv_lens_correction_ref(uv_handle sdk_handle,  uv_image *test_image, uv_image *neon_image_out, int *lookup_table);

#endif

