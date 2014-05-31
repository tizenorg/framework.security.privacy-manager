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


#ifndef __TIZEN_SECURITY_PRIVACY_MANAGER_H
#define __TIZEN_SECURITY_PRIVACY_MANAGER_H

#include <tizen.h>
#include <privacy_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_PRIVACY_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumerations of error code for Privacy Manager.
 */
typedef enum
{
	PRIVACY_MANAGER_ERROR_NONE				= TIZEN_ERROR_NONE,	/**< Successful */
	PRIVACY_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY		= TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	PRIVACY_MANAGER_ERROR_IO_ERROR			= TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	PRIVACY_MANAGER_ERROR_NO_DATA			= TIZEN_ERROR_NO_DATA, /**< No data found */
	PRIVACY_MANAGER_ERROR_DB_FAILED			= TIZEN_ERROR_APPLICATION_CLASS | 0x01, /**< Database error  */
	PRIVACY_MANAGER_ERROR_IPC_FAILED		= TIZEN_ERROR_APPLICATION_CLASS | 0x02, /**< Failed to communication with server */
} privacy_manager_error_e;

//struct privacy_info_h;

/**
 * @brief Called to get a package id once for each privacy package.
 * @remarks @a package_id will be automatically destroyed when the callback function returns. (Do not release @a package_id.)
 * @param[in] package_id The privacy package id
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre privacy_manager_foreach_privacy_packages() will invoke this callback.
 * @see privacy_manager_foreach_privacy_packages()
 */
typedef bool (*privacy_manager_privacy_packages_cb) (const char *package_id, void* user_data);

/**
 * @brief Called to get package privacy info once for specific package.
 * @remarks @a privacy_info will be automatically destroyed when the callback function returns. (Do not release @a privacy_info.)
 * @param[in] privacy_info_h privacy info of the packcage
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre privacy_manager_foreach_privacy_info() will invoke this callback.
 * @see privacy_manager_foreach_privacy_info()
 */
typedef bool (*privacy_manager_privacy_info_cb) (privacy_info_h privacy_info, void* user_data);

/**
 * @brief Called to get all privacy info once.
 * @remarks @a privacy_info will be automatically destroyed when the callback function returns. (Do not release @a privacy_info.)
 * @param[in] privacy_info_h privacy info
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre privacy_manager_foreach_all_privacy() will invoke this callback.
 * @see privacy_manager_foreach_all_privacy()
 */
typedef bool (*privacy_manager_all_privacy_info_cb) (privacy_info_h privacy_info, void* user_data);

/**
 * @brief Called to get a package id and privacy enabled info once for each privacy ID.
 * @remarks @a package_id will be automatically destroyed when the callback function returns. (Do not release @a package_id.)
 * @param[in] package_id The package id which uses the specific privacy information.
 * @param[in] is_enabled A Boolean value indicating whether the privacy setting of the package is enabled
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre privacy_manager_foreach_package_list_by_privacy() will invoke this callback.
 * @see privacy_manager_foreach_package_list_by_privacy()
 */
typedef bool (*privacy_manager_packages_by_privacy_cb) (const char *package_id, bool is_enabled, void* user_data);

/**
 * @brief Retrieves privacy package list.
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_NO_DATA no package founded.
 * @retval #PRIVACY_MANAGER_ERROR_IPC_FAILED IPC failed
 * @retval #PRIVACY_MANAGER_ERROR_DB_FAILED DB operation failed
 * @post	This function invokes privacy_manager_privacy_packages_cb() repeatedly for each application context.
 * @see privacy_manager_privacy_packages_cb()
 */
EXPORT_API int privacy_manager_foreach_privacy_packages(privacy_manager_privacy_packages_cb callback, void *user_data);

/**
 * @brief Retrieves privacy info of the pacakge.
 * @param [in] package_id The ID of the pacakge.
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_NO_DATA no privacy info founded.
 * @retval #PRIVACY_MANAGER_ERROR_IPC_FAILED IPC failed
 * @retval #PRIVACY_MANAGER_ERROR_DB_FAILED DB operation failed
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER invalid parameter
 * @post	This function invokes privacy_manager_privacy_info_cb() repeatedly for each application context.
 * @see privacy_manager_privacy_info_cb()
 */
EXPORT_API int privacy_manager_foreach_privacy_info(const char *package_id, privacy_manager_privacy_info_cb callback, void* user_data);

/**
 * @brief Enables or disables privacy of the package.
 * @param [in] package_id The ID of the pacakge.
 * @param [in] privacy_id The ID of the privacy to be changed.
 * @param [in] callback The callback function to invoke
 * @param [in] enable A Boolean value indicating whether the privacy setting is enabled
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_IPC_FAILED IPC failed
 * @retval #PRIVACY_MANAGER_ERROR_DB_FAILED DB operation failed
 * @retval #PRIVACY_MANAGER_ERROR_INVALID_PARAMETER invalid parameter
 */
EXPORT_API int privacy_manager_set_package_privacy(const char *package_id, const char *privacy_id, bool enable);

/**
 * @brief Retrieves all privacy list.
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_IPC_FAILED IPC failed
 * @retval #PRIVACY_MANAGER_ERROR_DB_FAILED DB operation failed
 * @post	This function invokes privacy_manager_all_privacy_info_cb() repeatedly for each application context.
 * @see privacy_manager_all_privacy_info_cb()
 */
EXPORT_API int privacy_manager_foreach_all_privacy(privacy_manager_all_privacy_info_cb callback, void* user_data);

/**
 * @brief Retrieves package list by the privacy id.
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRIVACY_MANAGER_ERROR_NONE Successful
 * @retval #PRIVACY_MANAGER_ERROR_NO_DATA no package founded.
 * @retval #PRIVACY_MANAGER_ERROR_IPC_FAILED IPC failed
 * @retval #PRIVACY_MANAGER_ERROR_DB_FAILED DB operation failed
 * @post	This function invokes privacy_manager_all_privacy_info_cb() repeatedly for each application context.
 * @see privacy_manager_all_privacy_info_cb()
 */
EXPORT_API int privacy_manager_foreach_package_list_by_privacy(const char *privacy_id, privacy_manager_packages_by_privacy_cb callback, void* user_data);



#ifdef __cplusplus
}
#endif

#endif //__TIZEN_PRIVACYMGR_PRIVACY_MANAGER_H
