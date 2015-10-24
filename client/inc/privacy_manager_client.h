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

#ifndef _PRIVACY_MANAGER_CLIENT_H
#define _PRIVACY_MANAGER_CLIENT_H

#include <privacy_info_client.h>
#include <privacy_manager_client_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*privacy_manager_client_privacy_packages_cb) (const char *package_id, void* user_data);
typedef bool (*privacy_manager_client_privacy_info_cb) (privacy_info_client_s* privacy_info, void* user_data);
typedef bool (*privacy_manager_client_all_privacy_info_cb) (privacy_info_client_s* privacy_info, void* user_data);
typedef bool (*privacy_manager_client_packages_by_privacy_cb) (const char *package_id, bool is_enabled, void* user_data);

EXTERN_API int privacy_manager_client_install_privacy(const char *package_id, const char** privacy_list, bool privacy_popup_required);
EXTERN_API int privacy_manager_client_install_privacy_by_client(const char *package_id, const char** privacy_list, bool privacy_popup_required);
EXTERN_API int privacy_manager_client_uninstall_privacy(const char *package_id);
EXTERN_API int privacy_manager_client_uninstall_privacy_by_server(const char *package_id);
EXTERN_API int privacy_manager_client_foreach_privacy_packages(privacy_manager_client_privacy_packages_cb callback, void *user_data);
EXTERN_API int privacy_manager_client_foreach_get_privacy_info(const char *package_id, privacy_manager_client_privacy_info_cb callback, void* user_data);
EXTERN_API int privacy_manager_client_set_package_privacy(const char *package_id, const char *privacy_id, bool enable);

EXTERN_API int privacy_manager_client_check_user_consented(const char *package_id, bool *consented);
EXTERN_API int privacy_manager_client_set_user_consented(const char *package_id, bool consented);

EXTERN_API int privacy_manager_client_foreach_all_privacy(privacy_manager_client_all_privacy_info_cb callback, void* user_data);
EXTERN_API int privacy_manager_client_foreach_package_list_by_privacy(const char *privacy_id, privacy_manager_client_packages_by_privacy_cb callback, void* user_data);

EXTERN_API int privacy_manager_client_update_privacy(const char *package_id, const char** privacy_list);

#ifdef __cplusplus
}
#endif


#endif //__TIZEN_PRIVACYMGR_PRIVACY_MANAGER_CLIENT_H

