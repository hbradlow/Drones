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

#include "helpers.h"
#include <uv_types.h>
#include <uv_errors.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdarg.h>
#include "image_utils.h"

#define MAX_LOG_STR 2048

#ifdef __APPLE__
// Definitions provided in the App code from XCode/ObjC side
double UViOSCurrentTimeMillis();
void UViOSLogCallback(const char*);
#endif // __APPLE__

static char *psBaseOutputPath = NULL;
int uv_set_output_base_path(const char* pBaseOutputPath) {
    psBaseOutputPath = (char*)pBaseOutputPath; // There is a const issue here - Just for now - TBD
    return UV_E_SUCCESS;
}

int uv_printf(const char * format, ...)
{
	int result = 0;
	va_list args;
	va_start (args, format);
#ifdef __APPLE__
    result = vprintf(format, args);
    static int maxLen = MAX_LOG_STR;
    static char msg[MAX_LOG_STR];
    vsnprintf(msg, maxLen-1, format, args);
    UViOSLogCallback(msg);
#else
//	if(! uv_is_JNI_active())
		result = vprintf(format, args);
/*	else
	{
		static int maxLen = MAX_LOG_STR;
		static char msg[MAX_LOG_STR];
		vsnprintf(msg, maxLen-1, format, args);
		uv_send_string_through_JNI(msg); // TBD - ret val
	}*/
#endif

	va_end (args);
	return result;
}
/*
double uv_now_ms()
{
#ifdef __APPLE__
    return UViOSCurrentTimeMillis();
#else
    struct timespec res;
    #if _POSIX_TIMERS > 0
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
    #else
    //#ifndef __APPLE__
    //struct timeval tv;
    //gettimeofday(&tv, NULL);
    //return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //res.tv_sec = tv.tv_sec;
    //res.tv_nsec = tv.tv_usec*1000;
    #endif
#endif //__APPLE__
}
*/
double uv_now_ms()
{
#ifdef __APPLE__
    return UViOSCurrentTimeMillis();
#else
    struct timespec res;
//    #if _POSIX_TIMERS > 0
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
//    #else
    //#ifndef __APPLE__
    //struct timeval tv;
    //gettimeofday(&tv, NULL);
    //return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //res.tv_sec = tv.tv_sec;
    //res.tv_nsec = tv.tv_usec*1000;
//    #endif
#endif //__APPLE__
}
void uv_flush_cache(char* begin, char *end, int param)
{
	/*const int syscall = 0xf0002;
	__asm __volatile (
		"mov	 r0, %0\n"
		"mov	 r1, %1\n"
		"mov	 r7, %2\n"
		"mov     r2, #0x0\n"
		"svc     0x00000000\n"
		:
		:	"r" (begin), "r" (end), "r" (syscall)
		:	"r0", "r1", "r7"
		);*/
    //cacheflush(begin, end, 2);
    //__clear_cache(begin, end);
     system("sync; echo 3 > /proc/sys/vm/drop_caches");

}

uv_error uv_create_output_name(char *name, char* suffix, char** name_ouput)
{
    char *pBaseLocation = psBaseOutputPath ? psBaseOutputPath : OUTPUT_LOCATION;
    int name_len = strlen(name) + strlen(pBaseLocation) + 1;
    int len = name_len + 1 + strlen(suffix);
    char *new_name = malloc(sizeof(char) * len);
    if(!new_name)
        return UV_E_OUT_OF_MEMORY;

    strcpy(new_name, pBaseLocation);
    strcat(new_name, "/");
    strcat(new_name, name);
    new_name[name_len - 4] = '\0';

    strcat(new_name, suffix);
    *name_ouput = new_name;

	return UV_E_SUCCESS;
}

uv_error uv_append_path_load_image(const char* base_path, const char *filename, uv_image *image_out)
{
    char full_file_name[512];
    strcpy(full_file_name, base_path);
    strcat(full_file_name, filename);
    return uv_LoadImage(full_file_name, image_out);
}

void sort_files(char *files[], int max_file)
{
	char temp[50];
	int i,j;
	for(i=0; i<max_file; i++ )
		for(j=i+1; j<max_file; j++)
		{

			if(strcmp(files[i], files[j]) > 0)
				{
					strcpy(temp, files[i]);
					strcpy(files[i], files[j]);
					strcpy(files[j], temp);
				}
		}

//	for(i=0; i<max_file; i++)
//		printf("%s\n", files[i]);
}


uv_error uv_create_input_list(char* base_path, int* max_files, char *files[])
{
	int i = 0;
    //int n;
    DIR *dir = NULL;
    struct dirent *drnt = NULL;
    //char **files = NULL;

    //files = (char**) malloc(max_files * sizeof(char*));
    dir = opendir(base_path);
    if(!dir)
        return UV_E_INVALID_INPUT;

    while((drnt = readdir(dir)) && (i < *max_files))
    {
        if( (strcmp(drnt->d_name, ".") == 0) || (strcmp(drnt->d_name, "..") == 0) )
            continue;
        //if(drnt -> d_name[6] != 'r')// && drnt -> d_name[7] == 'g' && drnt -> d_name[8] == 'b')
         //   continue;

        files[i] = (char*) malloc((strlen(drnt->d_name) + 1) * sizeof(char));
        strcpy(files[i], drnt->d_name);
        i++;
    }
    *max_files = i;

    closedir(dir);
sort_files(files, i);
    return UV_E_SUCCESS;
}



