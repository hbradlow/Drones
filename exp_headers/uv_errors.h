/*
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

#ifndef UV_ERRORS_H
#define UV_ERRORS_H

#include <string.h>

//#define UV_ENABLE_VALIDATION_NEON 1 ///* Validation condition
//#define UV_ENABLE_VALIDATION_REF 1 ///* Validation condition
//
////#define APK_LIB
////
////#ifndef APK_LIB
//
//#define UV_ENABLE_VALID_COUNT_NEON 1 ///* Validation Number of runs
//#define UV_ENABLE_VALID_COUNT_REF 1 ///* Validation Number of runs
//
//#define MAX_CALLS_NEON 1200
//#define MAX_CALLS_REF 1200
//
////#endif

typedef unsigned int uv_error;

/** Success */
#define UV_E_SUCCESS           0

/** Memory allocation failed  */
#define UV_E_OUT_OF_MEMORY     100

/** Internal Error. You need to contact support */
#define UV_E_INTERNAL          101

/** Invalid Input */
#define UV_E_INVALID_INPUT     102

/** Unsupported method or feature */
#define UV_E_UNSUPPORTED		103

/** Unable to find a file */
#define UV_E_FILE_NOT_FOUND		104

/** Licence has expired */
#define UV_E_LIC_EXPIRED		105

/** Licence verification failed */
#define UV_E_SIG_VERIFICATION		106

/** SDK handle is invalid */
#define UV_E_INVALID_HANDLE     107

/** MAX calls is invalid */
#define UV_MAX_CALLS_INVALID     108

#endif /* UV_ERRORS_H */
