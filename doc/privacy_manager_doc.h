/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __TIZEN_CORE_LIB_PRIVACY_MANAGER_DOC_H__
#define __TIZEN_CORE_LIB_PRIVACY_MANAGER_DOC_H__

/**
 * @defgroup CAPI_PRIVACY_MANAGER_MODULE Privacy Manager
 * @ingroup CAPI_SECURITY_FRAMEWORK
 * @brief    The Privacy Manager provides functions for getting package list which can access user's privacy information and getting privacy setting information of the system.
 * If the user turned off a privacy setting to the specific package, the package can not access related user's privacy information.
 * @section CAPI_PRIVACY_MANAGER_MODULE_HEADER Required Header
 *   #include <privacy_manager.h>
 *
 * @section CAPI_PRIVACY_MANAGER_MODULE_OVERVIEW Overview
 * This module provides information about package id can access user's privacy information and it's privacy setting information(@see privacy_info).
 */

/**
 * @defgroup CAPI_PRIVACY_INFO_MODULE Privacy Information
 * @ingroup CAPI_PRIVACY_MANAGER_MODULE
 * @brief    The Privacy Information provides functions for getting it's privacy setting information, privacy id, display name and so on.
 * @section CAPI_PRIVACY_INFO_MODULE_HEADER Required Header
 *   #include <privacy_manager.h>
 *
 * @section CAPI_PRIVACY_INFO_MODULE_OVERVIEW Overview
 * This module provides functions for:
 *    - releasing privacy information and all its resources (privacy_info_destroy()),
 *    - getting the privacy ID/display name of the given privacy info (privacy_info_get_privacy_id(), privacy_info_get_privacy_display_name(), privacy_info_get_privacy_display_name_string_id()),
 *    - getting the description of the given privacy info (privacy_info_get_privacy_description(), privacy_info_get_privacy_description_string_id()),
 *    - checking whether the privacy setting is enabled (privacy_info_is_enabled()),
 *    - creating a copy of the given privacy info (privacy_info_clone()).
 */

#endif
