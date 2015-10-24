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

#include <string.h>
#include <string>
#include <memory>
#include <PrivacyManagerClient.h>
#include <privacy_manager_client.h>
#include <privacy_manager_client_types.h>
#include <Utils.h>
#include "privacy_manager_client_internal_types.h"

int privacy_info_client_s_destroy(privacy_info_client_s* privacy_info)
{
	if (privacy_info != NULL)
	{
		if (privacy_info->privacy_id)
			free(privacy_info->privacy_id);
		free (privacy_info);
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_get_privacy_id(privacy_info_client_s* privacy_info, char **privacy_id)
{
	int size = strlen(privacy_info->privacy_id);
	*privacy_id = (char*) calloc(1, size + 1);
	TryReturn(*privacy_id != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, ,"[PRIV_MGR_ERROR_OUT_OF_MEMORY] privacy_id's calloc Failed");
	memcpy (*privacy_id, privacy_info->privacy_id, size);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_get_privacy_display_name(privacy_info_client_s* privacy_info, char **name)
{
	std::string displayName;

	int res = PrivacyManagerClient::getInstance()->getPrivaycDisplayName(std::string(privacy_info->privacy_id), displayName);
	if (res != PRIV_MGR_ERROR_SUCCESS)
		return res;

	int size = strlen(displayName.c_str());
	*name = (char*) calloc(1, size + 1);
	TryReturn(*name != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, ,"[PRIV_MGR_ERROR_OUT_OF_MEMORY] name's calloc Failed");
	memcpy (*name, displayName.c_str(), size);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_get_privacy_display_name_string_id(privacy_info_client_s* privacy_info, char **name_string_id)
{
	std::string displayNameStringId;
	int res = PrivacyManagerClient::getInstance()->getPrivaycDisplayNameStringId(std::string(privacy_info->privacy_id), displayNameStringId);
	if (res != PRIV_MGR_ERROR_SUCCESS)
		return res;

	int size = strlen(displayNameStringId.c_str());
	*name_string_id = (char*) calloc(1, size + 1);
	TryReturn(*name_string_id != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, ,"[PRIV_MGR_ERROR_OUT_OF_MEMORY] name_string_id's calloc Failed");
	memcpy (*name_string_id, displayNameStringId.c_str(), size);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_get_privacy_description(privacy_info_client_s* privacy_info, char **description)
{
	std::string desc;
	int res = PrivacyManagerClient::getInstance()->getPrivaycDisplayName(std::string(privacy_info->privacy_id), desc);
	if (res != PRIV_MGR_ERROR_SUCCESS)
		return res;

	int size = strlen(desc.c_str());
	*description = (char*) calloc(1, size + 1);
	TryReturn(*description != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, ,"[PRIV_MGR_ERROR_OUT_OF_MEMORY] description's calloc Failed");
	memcpy (*description, desc.c_str(), size);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_get_privacy_description_string_id(privacy_info_client_s* privacy_info, char **description_string_id)
{
	std::string descStringId;
	int res = PrivacyManagerClient::getInstance()->getPrivaycDisplayName(std::string(privacy_info->privacy_id), descStringId);
	if (res != PRIV_MGR_ERROR_SUCCESS)
		return res;

	int size = strlen(descStringId.c_str());
	*description_string_id = (char*) calloc(1, size + 1);
	TryReturn(*description_string_id != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, ,"[PRIV_MGR_ERROR_OUT_OF_MEMORY] description_string_id's calloc Failed");
	memcpy (*description_string_id, descStringId.c_str(), size);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_client_is_enabled(privacy_info_client_s* privacy_info, bool *enabled)
{
	*enabled = privacy_info->is_enabled;
	return PRIV_MGR_ERROR_SUCCESS;
}
