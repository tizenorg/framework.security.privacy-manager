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
#include <stdlib.h>
#include <privacy_manager.h>
#include <privacy_info.h>
#include <privacy_manager_client_types.h>
#include <privacy_info_client.h>
#include <privacy_manager_client.h>
#include <privacy_manager_types_private.h>
#include <dlog.h>


static int _privacy_manager_convert_error(int error)
{
	int ret = PRIV_MGR_ERROR_SUCCESS;

	LOGD("before error : %d", error);

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
	LOGD("after error : %d", ret);
	return ret;
}

typedef struct _foreach_privacy_package_context_
{
	privacy_manager_privacy_packages_cb callback;
	void *user_data;
} foreach_privacy_package_context_s;

typedef struct _foreach_privacy_info_context_
{
	privacy_manager_privacy_info_cb callback;
	void *user_data;
} foreach_privacy_info_context_s;

typedef struct _foreach_all_privacy_context_
{
	privacy_manager_all_privacy_info_cb callback;
	void *user_data;
} foreach_all_privacy_context_s;

typedef struct _foreach_package_by_privacy_context_
{
	privacy_manager_packages_by_privacy_cb callback;
	void *user_data;
} foreach_package_by_privacy_context_s;

//static int _create_privacy_info(const char* package_id, bool enabled, privacy_info_h *privacy_info)
//{
//	privacy_info_h temp = (privacy_info_h) calloc(1, sizeof(privacy_info_h));
//	if (temp == NULL)
//		return PRIV_MGR_ERROR_OUT_OF_MEMORY;
//
//	int size = strlen(package_id);
//	temp->privacy_info_client->privacy_id = (char*) calloc(1, size + 1);
//	if (temp->privacy_info_client->privacy_id == NULL)
//	{
//		free(temp);
//		return PRIV_MGR_ERROR_OUT_OF_MEMORY;
//	}
//	memcpy(temp->privacy_info_client->privacy_id, package_id, size + 1);
//
//	temp->privacy_info_client->is_enabled = enabled;
//
//	*privacy_info = temp;
//
//	return PRIV_MGR_ERROR_SUCCESS;
//}

bool __privacy_manager_client_privacy_packages_cb(const char *package_id, void* user_data)
{
	foreach_privacy_package_context_s* context = (foreach_privacy_package_context_s*) user_data;
	return context->callback(package_id, context->user_data);
}

bool __privacy_manager_client_privacy_info_cb (privacy_info_client_s* privacy_info_client, void* user_data)
{
	foreach_privacy_info_context_s* context = (foreach_privacy_info_context_s*) user_data;

	struct privacy_info_s* privacy_info = NULL;

	bool ret = false;

	privacy_info = (struct privacy_info_s*) calloc(1, sizeof(struct privacy_info_s));

	privacy_info->privacy_info_client = privacy_info_client;

	ret = context->callback(privacy_info, context->user_data);

	free(privacy_info);

	return ret;
}

bool __privacy_manager_client_all_privacy_info_cb(privacy_info_client_s* privacy_info_client, void* user_data)
{
	foreach_all_privacy_context_s* context = (foreach_all_privacy_context_s*) user_data;

	struct privacy_info_s* privacy_info = NULL;

	bool ret = false;

	privacy_info = (struct privacy_info_s*) calloc(1, sizeof(struct privacy_info_s));

	privacy_info->privacy_info_client = privacy_info_client;

	ret = context->callback(privacy_info, context->user_data);

	free(privacy_info);

	return ret;
}

bool __privacy_manager_client_packages_by_privacy_cb(const char *package_id, bool is_enabled, void* user_data)
{
	foreach_package_by_privacy_context_s* context = (foreach_package_by_privacy_context_s*) user_data;

	bool ret = false;

	ret = context->callback(package_id, is_enabled, context->user_data);

	return ret;
}

int privacy_manager_foreach_privacy_packages(privacy_manager_privacy_packages_cb callback, void *user_data)
{
	int retval;

	foreach_privacy_package_context_s* context = NULL;

	context = (foreach_privacy_package_context_s*) calloc(1, sizeof(foreach_privacy_package_context_s));
	if (context == NULL)
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;

	context->callback = callback;
	context->user_data = user_data;

	retval = privacy_manager_client_foreach_privacy_packages(__privacy_manager_client_privacy_packages_cb, context);
	free(context);

	return _privacy_manager_convert_error(retval);
}

int privacy_manager_foreach_privacy_info(const char *package_id, privacy_manager_privacy_info_cb callback, void* user_data)
{
	int retval;

	foreach_privacy_info_context_s* context = NULL;
	
	context = (foreach_privacy_info_context_s*) calloc(1, sizeof(foreach_privacy_info_context_s));
	if (context == NULL)
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;

	context->callback = callback;
	context->user_data = user_data;

	retval = privacy_manager_client_foreach_get_privacy_info(package_id, __privacy_manager_client_privacy_info_cb, context);

	return _privacy_manager_convert_error(retval);
}

int privacy_manager_set_package_privacy(const char *package_id, const char *privacy_id, bool enable)
{
	int retval = privacy_manager_client_set_package_privacy(package_id, privacy_id, enable);

	return _privacy_manager_convert_error(retval);
}

int privacy_manager_foreach_all_privacy(privacy_manager_all_privacy_info_cb callback, void* user_data)
{
	int retval;

	foreach_all_privacy_context_s* context = NULL;
	
	context = (foreach_all_privacy_context_s*) calloc(1, sizeof(foreach_all_privacy_context_s));
	if (context == NULL)
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;

	context->callback = callback;
	context->user_data = user_data;

	retval = privacy_manager_client_foreach_all_privacy(__privacy_manager_client_all_privacy_info_cb, context);

	return _privacy_manager_convert_error(retval);
}

int privacy_manager_foreach_package_list_by_privacy(const char *privacy_id, privacy_manager_client_packages_by_privacy_cb callback, void* user_data)
{
	int retval;

	foreach_package_by_privacy_context_s* context = NULL;
	
	context = (foreach_package_by_privacy_context_s*) calloc(1, sizeof(foreach_package_by_privacy_context_s));
	if (context == NULL)
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;

	context->callback = callback;
	context->user_data = user_data;

	retval = privacy_manager_client_foreach_package_list_by_privacy(privacy_id, __privacy_manager_client_packages_by_privacy_cb, context);

	return _privacy_manager_convert_error(retval);
}