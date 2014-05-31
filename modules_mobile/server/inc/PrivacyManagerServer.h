/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef _PRIVACY_MANAGER_SERVER_H_
#define _PRIVACY_MANAGER_SERVER_H_

#include <string>
#include <memory>
#include <list>
#include <mutex>
#include <NotificationServer.h>

class NotificationServer;

class PrivacyManagerServer
{
private:
	static std::mutex m_singletonMutex;
	static PrivacyManagerServer* m_pInstance;
	NotificationServer m_notificationServer;

private:
	void createDB(void);

public:

	explicit PrivacyManagerServer(void);

	static PrivacyManagerServer* getInstance(void);

	int getPrivacyAppPackages(std::list <std::string>& list);

	int getAppPackagePrivacyInfo(const std::string pkgId, std::list < std::pair < std::string, bool > > & list);

	int setPrivacySetting(const std::string pkgId, const std::string privacyId, bool enabled);

	int addAppPackagePrivacyInfo(const std::string pkgcId, const std::list < std::string > privilegeList, bool privacyPopupRequired);

	int removeAppPackagePrivacyInfo(const std::string pkgId);

	int isUserPrompted(const std::string pkgId, bool& isPrompted);

	int setUserPrompted(const std::string pkgId, bool prompted);

	int setPermissions(const std::string pkgId, const std::string privacyId, bool enabled);

};


#endif // _PRIVACY_MANAGER_SERVER_H_