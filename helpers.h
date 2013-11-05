/**
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

#ifndef UV_HELPER_H
#define UV_HELPER_H

#include <uv_types.h>
#include <uv_errors.h>

#include <uv_types1.h>

#define LIC_FILE "assets/licence.lic"
#define LIC_SIG "assets/licence.sig"

//#define OUTPUT_REF_SUFFIX "_output_ref.bin"
//#define OUTPUT_NEON_SUFFIX "_output_neon.bin"

#define OUTPUT_REF_SUFFIX "_output_ref"
#define OUTPUT_NEON_SUFFIX "_output_neon"

#ifdef __APPLE__
    #define INPUT_BASEPATH ""
    #define OUTPUT_BASEPATH ""
#else
   // #define INPUT_BASEPATH "/mnt/sdcard/uvvision"
    #define INPUT_BASEPATH "./uvvision"
    #define OUTPUT_BASEPATH INPUT_BASEPATH
#endif

#define RGB_INPUT_LOCATION INPUT_BASEPATH "/rgb_input/"
#define GRAY_INPUT_LOCATION INPUT_BASEPATH "/gray_input/"
#define THRESHOLD_INPUT_LOCATION INPUT_BASEPATH "/threshold_input/"
#define TRACKING_INPUT_LOCATION INPUT_BASEPATH "/tracking_input/"

#define OUTPUT_LOCATION OUTPUT_BASEPATH "/output/"

//#define RGB_INPUT_LOCATION "./uvvision/rgb_input/"
//#define GRAY_INPUT_LOCATION "./uvvision/gray_input/"
//#define THRESHOLD_INPUT_LOCATION "./uvvision/threshold_input/"
//#define TRACKING_INPUT_LOCATION "./uvvision/tracking_input/"
//#define OUTPUT_LOCATION "./uvvision/output/"

//#define RGB_INPUT_LOCATION "/data/rgb_input/"
//#define GRAY_INPUT_LOCATION "/data/gray_input/"
//#define THRESHOLD_INPUT_LOCATION "/data/threshold_input/"
//#define TRACKING_INPUT_LOCATION "/data/tracking_input/"
//#define OUTPUT_LOCATION "/data/output/"

#define MAX_FILES 2
#define DEFAULT_ITERATIONS 10
//#define RANDOM_IMAGE
#define NO_REFERENCE_PRINT


#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if !(TARGET_IPHONE_SIMULATOR)
        #warning "not TARGET_IPHONE_SIMULATOR"
        #define HAVE_NEON 1
    #endif
#else
	#define HAVE_NEON 1
#endif


//#define UV_ENABLE_CACHE_FLUSH       //Comment this if it is to be disabled
#define _DBG_MEM_LEAK				//Comment this if it is to be disabled
#define _DBG_MEM_SIZE 640*480
#define MAX_TEST_SIZES 10			//Resize sizes for width and height
#define CHECK_ERROR_RETURN(err) \
	 if (err != UV_E_SUCCESS) \
	 { \
		return err; \
	 } \

double uv_now_ms();


typedef struct uv_test_input
{
    char* base_path;
    char* image_name_1;
    char* image_name_2;
} uv_test_input;

//
//typedef struct uv_testInput
//{
//    uv_image *inputImage;
//    int iterations;
//} uv_testInput;

//typedef struct uv_testOutput
//{
//    uv_image *outputRefImage;
//    uv_image *outputNeonImage;
//    int mismatch;
//} uv_testOutput;

void uv_flush_cache(char *begin, char *end, int param);
uv_error uv_create_output_name(char *name, char* suffix, char** name_ouput);
uv_error uv_create_input_list(char* base_path, int* max_files, char *files[]);
uv_error uv_append_path_load_image(const char* base_path, const char *filename, uv_image *image_out);
int uv_printf(const char * format, ...);
int uv_set_output_base_path(const char* pBaseOutputPath);
void uv_send_string_through_JNI(const char* msgStr);
int uv_is_JNI_active();

#endif /* UV_HELPER_H */
