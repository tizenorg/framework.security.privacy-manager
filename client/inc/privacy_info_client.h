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

#ifndef _PRIVACY_INFO_CLIENT_H
#define _PRIVACY_INFO_CLIENT_H

#include <privacy_manager_client_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _privacy_info_client_s {
	char *privacy_id;
	bool is_enabled;
} privacy_info_client_s;

EXTERN_API int create_privacy_info_client_s(const char* package_id, bool enabled, privacy_info_client_s **privacy_info);
EXTERN_API int privacy_info_client_s_destroy(privacy_info_client_s* privacy_info);
EXTERN_API int privacy_info_client_get_privacy_id(privacy_info_client_s* privacy_info, char **privacy_id);
EXTERN_API int privacy_info_client_get_privacy_display_name(privacy_info_client_s* privacy_info, char **name);
EXTERN_API int privacy_info_client_get_privacy_display_name_string_id(privacy_info_client_s* privacy_info, char **name_string_id);
EXTERN_API int privacy_info_client_get_privacy_description(privacy_info_client_s* privacy_info, char **description);
EXTERN_API int privacy_info_client_get_privacy_description_string_id(privacy_info_client_s* privacy_info, char **description_string_id);
EXTERN_API int privacy_info_client_is_enabled(privacy_info_client_s* privacy_info, bool *enabled);

#ifdef __cplusplus
}
#endif


#endif //_PRIVACY_INFO_CLIENT_H
