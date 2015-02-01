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
#include "privacy_manager_client_internal_types.h"
#include <PrivacyChecker.h>
#include <privacy_checker_client.h>
#include <privacy_info_client.h>
#include <PrivacyIdInfo.h>

int privacy_checker_initialize(const char *package_id)
{
	if(!package_id)
	{
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	}
	return PrivacyChecker::initialize(std::string(package_id));
}

int privacy_checker_check_by_privacy(const char *privacy_id)
{
	if(!privacy_id)
	{
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	}
	return PrivacyChecker::check(std::string(privacy_id));
}

int privacy_checker_check_by_privilege(const char *privilege_id)
{
	if(!privilege_id)
	{
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	}
	return PrivacyChecker::checkWithPrivilege(privilege_id);
}

int privacy_checker_finalize(void)
{
	return PrivacyChecker::finalize();
}

int privacy_checker_check_package_by_privilege(const char* package_id, const char *privilege_id)
{
	if(!package_id || !privilege_id)
	{
		return PRIV_MGR_ERROR_INVALID_PARAMETER;
	}
	return PrivacyChecker::checkWithPrivilege(package_id,privilege_id);
}
