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


#ifndef __TIZEN_SECURITY_PRIVACY_INFO_H
#define __TIZEN_SECURITY_PRIVACY_INFO_H

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Privacy information handle.
 */
typedef struct privacy_info_s *privacy_info_h;

/**
 * @brief Destroys the privacy information handle and releases all its resources.
 * @param [in] privacy_info The application information handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_manager_foreach_app_info()
 * @see app_manager_get_app_info()
 */
EXPORT_API int privacy_info_destroy(privacy_info_h privacy_info);

/**
 * @brief Gets the privacy ID with the given privacy info.
 * @remarks @a package must be released with free() by you.
 * @param [in] privacy_info The privacy information
 * @param [out] package_id The package ID of the privacy info
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
EXPORT_API int privacy_info_get_privacy_id(privacy_info_h privacy_info, char **privacy_id);

/**
 * @brief Gets the privacy display name with the given privacy info.
 * @remarks @a name must be released with free() by you.
 * @param [in] privacy_info The privacy information
 * @param [out] name The display name of the privacy info
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
EXPORT_API int privacy_info_get_privacy_display_name(privacy_info_h privacy_info, char **name);

/**
 * @brief Gets the description with the given privacy info.
 * @remarks @a description must be released with free() by you.
 * @param [in] privacy_info The privacy information
 * @param [out] description The description of the privacy info
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
EXPORT_API int privacy_info_get_privacy_description(privacy_info_h privacy_info, char **description);

/**
 * @brief Checks whether the privacy setting is enabled.
 * @param [in] privacy_info The privacy information
 * @param [out] enabled A boolean value indicating whether the privacy setting is enabled.
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
EXPORT_API int privacy_info_is_enabled(privacy_info_h privacy_info, bool *enabled);

/**
 * @brief Creates and returns a copy of the given privacy info handle.
 *
 * @remarks A newly created service should be destroyed by calling privacy_info_destroy() if it is no longer needed.
 *
 * @param [out] clone If successful, a newly created privacy info handle will be returned.
 * @param [in] privacy_info The privacy info handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @see privacy_info_destroy()
 */
EXPORT_API int privacy_info_clone(privacy_info_h *clone, privacy_info_h privacy_info);

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVACY_INFO_H
