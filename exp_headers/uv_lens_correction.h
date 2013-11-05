
#ifndef homo_graphic_neon
#define homo_graphic_neon


#include <uv_errors.h>
#include <uv_types.h>


uv_error uv_lens_correction ( uv_handle sdk_handle,  int *index_table, 
                              unsigned short int *fraction_table,uv_image *test_image, uv_image *neon_image_out);

#endif