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

#ifndef UV_VISION_H
#define UV_VISION_H

#include "uv_errors.h"
#include "uv_rgb2hsv.h"

/** Initialize the SDK. \n\n
  *
  * @param sdk_handle 		: The handle pointer. Will be initialized on successful return
  * @param lic_file_path	: Licence text file path
  * @param lic_sig_file_path: Licence signature file path
  * @param licFromZipFile: Set to 1 if the licence files paths are a zip
  *
  * @return #UV_E_SUCCESS if successful
  */
uv_error uv_initialize(uv_handle* sdk_handle, unsigned char* lic_file_path, unsigned char* lic_sig_file_path, int licFromZipFile, const char* APK_file_path);


/** UnInitialize the SDK. \n\n
  */
void uv_deinitialize(uv_handle sdk_handle);

#endif /* UV_VISION_H  */
