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
#include <dlog.h>
#include <PrivacyManagerClient.h>
#include <PrivacyIdInfo.h>
#include <privacy_manager_client.h>
#include "privacy_manager_client_internal_types.h"

int create_privacy_info_client_s(const char* privacy_id, bool enabled, privacy_info_client_s **privacy_info)
{
	privacy_info_client_s* temp = (privacy_info_client_s*) calloc(1, sizeof(privacy_info_client_s));
	if (temp == NULL)
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;

	int size = strlen(privacy_id);
	temp->privacy_id = (char*) calloc(1, size + 1);
	if (temp->privacy_id == NULL)
	{
		free(temp);
		return PRIV_MGR_ERROR_OUT_OF_MEMORY;
	}
	memcpy(temp->privacy_id, privacy_id, size + 1);
	
	temp->is_enabled = enabled;

	*privacy_info = temp;
	
	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_manager_client_install_privacy(const char *package_id, const char** privacy_list, bool privacy_popup_required)
{
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();
	std::list < std::string > privacyList;

	while (*privacy_list[0] != '\0')
		privacyList.push_back(std::string(*privacy_list++));

	int retval = pInst->addAppPackagePrivacyInfo(std::string(package_id), privacyList, privacy_popup_required, false);

	return retval;
}

int privacy_manager_client_uninstall_privacy(const char *package_id)
{
	if (package_id == NULL)
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	return PrivacyManagerClient::getInstance()->removeAppPackagePrivacyInfo(std::string(package_id), false);
}

int privacy_manager_client_uninstall_privacy_by_server(const char *package_id)
{
	if (package_id == NULL)
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	return PrivacyManagerClient::getInstance()->removeAppPackagePrivacyInfo(std::string(package_id), true);
}

int privacy_manager_client_foreach_privacy_packages(privacy_manager_client_privacy_packages_cb callback, void *user_data)
{
	int retval;
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	std::list < std::string > list;
	retval = pInst->getPrivacyAppPackages(list);
	if (retval != PRIV_MGR_ERROR_SUCCESS)
		return retval;
	if (list.size() == 0)
		return PRIV_MGR_ERROR_NO_DATA;

	for (std::list < std::string >::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if ( ! callback(iter->c_str(), user_data) )
			break;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}
int privacy_manager_client_foreach_get_privacy_info(const char *package_id, privacy_manager_client_privacy_info_cb callback, void* user_data)
{
	int retval;
	bool res;
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	std::list < std::pair <std::string, bool > > list;

	retval = pInst->getAppPackagePrivacyInfo(std::string(package_id), list);
	if (retval != PRIV_MGR_ERROR_SUCCESS)
		return retval;
	if (list.size() == 0)
		return PRIV_MGR_ERROR_NO_DATA;

	for (std::list < std::pair <std::string, bool > >::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		privacy_info_client_s *privacy_info_client_s = NULL;
		retval = create_privacy_info_client_s(iter->first.c_str(), iter->second, &privacy_info_client_s);
		res = callback(privacy_info_client_s, user_data);
		privacy_info_client_s_destroy(privacy_info_client_s);
		if (!res)
			break;
	}
	return PRIV_MGR_ERROR_SUCCESS;

}
int privacy_manager_client_set_package_privacy(const char *package_id, const char *privacy_id, bool enable)
{
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	return pInst->setPrivacySetting(package_id, privacy_id, enable);
}

int privacy_manager_client_check_user_consented(const char *package_id, bool *consented)
{
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	return pInst->isUserPrompted(std::string(package_id), *consented);
}

int privacy_manager_client_set_user_consented(const char *package_id, bool consented)
{
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	return pInst->setUserPrompted(std::string(package_id), consented);
}

int privacy_manager_client_foreach_all_privacy(privacy_manager_client_all_privacy_info_cb callback, void* user_data)
{
	int retval;
	bool res;

	std::list < std::string > privacyList;
	retval = PrivacyIdInfo::getAllPrivacyId(privacyList);
	if (retval != PRIV_MGR_ERROR_SUCCESS)
		return retval;
	if (privacyList.size() == 0)
		return PRIV_MGR_ERROR_NO_DATA;

	for (std::list < std::string >::iterator iter = privacyList.begin(); iter != privacyList.end(); ++iter)
	{
		privacy_info_client_s *privacy_info_client_s = NULL;
		retval = create_privacy_info_client_s(iter->c_str(), false, &privacy_info_client_s);
		res = callback(privacy_info_client_s, user_data);
		privacy_info_client_s_destroy(privacy_info_client_s);
		if (!res)
			break;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int privacy_manager_client_foreach_package_list_by_privacy(const char *privacy_id, privacy_manager_client_packages_by_privacy_cb callback, void* user_data)
{
	int retval;
	bool res;
	PrivacyManagerClient* pInst = PrivacyManagerClient::getInstance();

	std::list < std::pair < std::string, bool > > packageList;
	retval = pInst->getAppPackagesbyPrivacyId(std::string(privacy_id), packageList);

	if (retval != PRIV_MGR_ERROR_SUCCESS)
		return retval;
	if (packageList.size() == 0)
		return PRIV_MGR_ERROR_NO_DATA;

	for (std::list < std::pair < std::string, bool > >::iterator iter = packageList.begin(); iter != packageList.end(); ++iter)
	{
		res = callback(iter->first.c_str(), iter->second, user_data);
		if (!res)
			break;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}