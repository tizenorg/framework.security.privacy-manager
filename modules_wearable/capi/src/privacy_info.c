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

#include <stdlib.h>
#include <privacy_manager.h>
#include <privacy_info.h>
#include <privacy_manager_client.h>
#include <privacy_manager_client_types.h>
#include <privacy_info_client.h>
#include <privacy_manager_types_private.h>

static int _privacy_info_convert_error(int error)
{
	int ret = PRIV_MGR_ERROR_SUCCESS;

	switch (error) 
	{
	case PRIV_MGR_ERROR_SUCCESS:
		ret = PRIVACY_MANAGER_ERROR_NONE;
		break;
	case PRIV_MGR_ERROR_INVALID_PARAMETER:
		ret = PRIVACY_MANAGER_ERROR_INVALID_PARAMETER;
		break;
	case PRIV_MGR_ERROR_OUT_OF_MEMORY:
		ret = PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY;
		break;
	case PRIV_MGR_ERROR_IO_ERROR:
		ret = PRIVACY_MANAGER_ERROR_IO_ERROR;
		break;
	case PRIV_MGR_ERROR_NO_DATA:
		ret = PRIVACY_MANAGER_ERROR_NO_DATA;
		break;
	case PRIV_MGR_ERROR_DB_ERROR:
		ret = PRIVACY_MANAGER_ERROR_DB_FAILED;
		break;
	case PRIV_MGR_ERROR_IPC_ERROR:
		ret = PRIVACY_MANAGER_ERROR_IPC_FAILED;
		break;
	}
	return ret;
}

int privacy_info_destroy(privacy_info_h privacy_info)
{
	int retval;
	if (privacy_info == NULL)
		return PRIV_MGR_ERROR_INVALID_PARAMETER;

	if (privacy_info->privacy_info_client)
	{
		retval = privacy_info_client_s_destroy(privacy_info->privacy_info_client);
		if (retval != PRIV_MGR_ERROR_SUCCESS)
			return retval;
	}

	free (privacy_info);

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_info_get_privacy_id(privacy_info_h privacy_info, char **package)
{
	int retval = privacy_info_client_get_privacy_id(privacy_info->privacy_info_client, package);

	return _privacy_info_convert_error(retval);
}

int privacy_info_get_privacy_display_name(struct privacy_info_s* privacy_info, char **name)
{
	int retval = privacy_info_client_get_privacy_display_name(privacy_info->privacy_info_client, name);

	return  _privacy_info_convert_error(retval);
}

int privacy_info_get_privacy_description(struct privacy_info_s* privacy_info, char **description)
{
	int retval = privacy_info_client_get_privacy_description(privacy_info->privacy_info_client, description);

	return  _privacy_info_convert_error(retval);
}

int privacy_info_is_enabled(struct privacy_info_s* privacy_info, bool *enable)
{
	int retval = privacy_info_client_is_enabled(privacy_info->privacy_info_client, enable);

	return  _privacy_info_convert_error(retval);
}

int privacy_info_create(const char *privacy_id, privacy_info_h *privacy_info)
{
	int retval;
	privacy_info_client_s* privacy_info_client = NULL;
	
	retval = create_privacy_info_client_s(privacy_id, false, &privacy_info_client);
	if  (retval != PRIV_MGR_ERROR_SUCCESS)
		return _privacy_info_convert_error(retval);

	privacy_info_h privacy_info_temp = NULL;
	privacy_info_temp = (struct privacy_info_s*) calloc(1, sizeof(struct privacy_info_s));
	if (privacy_info_temp == NULL)
	{
		privacy_info_client_s_destroy(privacy_info_client);
		return PRIVACY_MANAGER_ERROR_OUT_OF_MEMORY;
	}
	
	privacy_info_temp->privacy_info_client = privacy_info_client;

	*privacy_info = privacy_info_temp;

	return PRIVACY_MANAGER_ERROR_NONE;
}

int privacy_info_clone(privacy_info_h *clone, privacy_info_h privacy_info)
{
	int retval;
	if (clone == NULL || privacy_info == NULL)
		return PRIVACY_MANAGER_ERROR_INVALID_PARAMETER;
	
	retval = privacy_info_create(privacy_info->privacy_info_client->privacy_id, clone);

	return retval;
}