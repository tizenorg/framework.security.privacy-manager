/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _PRIVACY_CHECKER_CLIENT_H
#define _PRIVACY_CHECKER_CLIENT_H

#include <privacy_manager_client_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_PRIVACY_MANAGER_MODULE
 * @{
 */

/**
 * @brief Initialize privacy checker
 * @param [in] package_id The ID of the pacakge to check
 * @return PRIV_MGR_ERROR_SUCCESS on success, otherwise a negative error value
 * @retval #PRIV_MGR_ERROR_SUCCESS Successful
 * @retval #PRIV_MGR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PRIV_MGR_ERROR_SYSTEM_ERROR Failed to initialize
 * @post privacy_checker_finalize must be called to finalize
 */
EXTERN_API int privacy_checker_initialize(const char *package_id);

/**
 * @brief Checks privacy is enabled or not by privacy id
 * @param [in] privacy_ud The ID of privacy
 * @return PRIV_MGR_ERROR_SUCCESS if the privacy is enabled else PRIV_MGR_ERROR_USER_NOT_CONSENTED, otherwise a negative error value
 * @retval #PRIV_MGR_ERROR_SUCCESS Successful
 * @retval #PRIV_MGR_ERROR_NOT_INITIALIZED Privacy chcker is not initialized
 * @retval #PRIV_MGR_ERROR_DB_ERROR DB operation failed
 * @retval #PRIV_MGR_ERROR_USER_NOT_CONSENTED The privacy of the package disabled
 * @pre privacy_checker_initialize must be called to initialize
 * @post privacy_checker_finalize must be called to finalize
 * @see privacy_checker_initialize
 * @see privacy_checker_finalize
 */
EXTERN_API int privacy_checker_check_by_privacy(const char *privacy_id);

/**
 * @brief Checks privacy is enabled or not by privilege id
 * @param [in] privilege_id The ID of the privilege
 * @return PRIV_MGR_ERROR_SUCCESS if the privacy is enabled else PRIV_MGR_ERROR_USER_NOT_CONSENTED, otherwise a negative error value
 * @retval #PRIV_MGR_ERROR_SUCCESS Successful
 * @retval #PRIV_MGR_ERROR_NOT_INITIALIZED Privacy chcker is not initialized
 * @retval #PRIV_MGR_ERROR_DB_FAILED DB operation failed
 * @retval #PRIV_MGR_ERROR_INVALID_PARAMETER invalid parameter
 * @retval #PRIV_MGR_ERROR_USER_NOT_CONSENTED The privacy of the package disabled
 * @remark Caller can free resource by calling privacy_checker_finalize() after calling this API otherwise, finalize while library unloading\n
 * This API initialize privacy checker for the first time
 * @see privacy_checker_finalize
 */
EXTERN_API int privacy_checker_check_by_privilege(const char *privilege_id);

/**
 * @brief Finalize privacy checker
 * @return 0 on success.
 * @retval #PRIV_MGR_ERROR_SUCCESS Successful
 */
EXTERN_API int privacy_checker_finalize(void);

/**
 * @brief Check privacy is enabled or not by privilege id of the package
 * @param [in] package_id The ID of the pacakge
 * @param [in] privacy_id The ID of the privacy
 * @return PRIV_MGR_ERROR_SUCCESS if the privacy is enabled else PRIV_MGR_ERROR_USER_NOT_CONSENTED, otherwise a negative error value
 * @retval #PRIV_MGR_ERROR_SUCCESS Successful
 * @retval #PRIV_MGR_ERROR_DB_FAILED DB operation failed
 * @retval #PRIV_MGR_ERROR_INVALID_PARAMETER invalid parameter
 * @retval #PRIV_MGR_ERROR_USER_NOT_CONSENTED The privacy of the package disabled
 * @post privacy_checker_finalize must be called to finalize
 * @see privacy_checker_finalize
 */
EXTERN_API int privacy_checker_check_package_by_privilege(const char* package_id, const char *privilege_id);

#ifdef __cplusplus
}
#endif


#endif //_PRIVACY_CHECKER_CLIENT_H
