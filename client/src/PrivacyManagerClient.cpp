/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <PrivacyManagerClient.h>
#include <PrivacyDb.h>
#include <SocketClient.h>
#include <PrivacyIdInfo.h>
#include <algorithm>
#include <memory>
#include <Utils.h>

//#undef __READ_DB_IPC__
#define __READ_DB_IPC__


std::mutex PrivacyManagerClient::m_singletonMutex;
PrivacyManagerClient* PrivacyManagerClient::m_pInstance = NULL;
const std::string PrivacyManagerClient::INTERFACE_NAME("PrivacyInfoService");

PrivacyManagerClient::PrivacyManagerClient(void)
{
	std::unique_ptr<SocketClient> pSocketClient(new SocketClient(INTERFACE_NAME));
	m_pSocketClient = std::move(pSocketClient);
}

PrivacyManagerClient*
PrivacyManagerClient::getInstance(void)
{
	std::lock_guard<std::mutex> guard(m_singletonMutex);
	if (m_pInstance == NULL)
		m_pInstance = new PrivacyManagerClient();
	return m_pInstance;
}

int
PrivacyManagerClient::addAppPackagePrivacyInfo(const std::string pkgId, const std::list < std::string >& list, bool privacyPopupRequired, bool isServerOperation)
{

	std::list < std::string > privacyList;

	int res = PrivacyIdInfo::getPrivacyIdListFromPrivilegeList(list, privacyList);
	if (res != PRIV_MGR_ERROR_SUCCESS )
		return res;

	if (privacyList.size() == 0)
		return PRIV_MGR_ERROR_SUCCESS;

	if (isServerOperation == true)
	{
		int result = PRIV_MGR_ERROR_SUCCESS;
		res = m_pSocketClient->connect();
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
		res = m_pSocketClient->call("addPrivacyInfo", pkgId, privacyList, privacyPopupRequired, &result);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
		res = m_pSocketClient->disconnect();
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

		return result;
	}
	else
	{
		return PrivacyDb::getInstance()->addAppPackagePrivacyInfo(pkgId, privacyList, privacyPopupRequired);
	}
}

int
PrivacyManagerClient::removeAppPackagePrivacyInfo(const std::string pkgId, bool isServerOperation)
{
	if (isServerOperation == true)
	{
		int result = PRIV_MGR_ERROR_SUCCESS;
		int res = m_pSocketClient->connect();
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
		result = m_pSocketClient->call("removePrivacyInfo", pkgId, &result);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
		result = m_pSocketClient->disconnect();
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

		return result;
	}
	else
		return PrivacyDb::getInstance()->removeAppPackagePrivacyInfo(pkgId);
}

int
PrivacyManagerClient::setPrivacySetting(const std::string pkgId, const std::string privacyId, bool isEnabled)
{
	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = m_pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = m_pSocketClient->call("setPrivacySetting", pkgId, privacyId, isEnabled, &result);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = m_pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);

	return result;
}
	
int
PrivacyManagerClient::getPrivacyAppPackages(std::list < std::string >& list) const
{
#ifdef __READ_DB_IPC__

	std::unique_ptr <SocketClient> pSocketClient (new SocketClient(INTERFACE_NAME));

	int result = PRIV_MGR_ERROR_SUCCESS;
	int size = 0;

	int res = pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = pSocketClient->call("getPrivacyAppPackages", &result, &size, &list);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);

	return result;
#else
	return PrivacyDb::getInstance()->getPrivacyAppPackages(list);
#endif
}

int
PrivacyManagerClient::getAppPackagePrivacyInfo(const std::string pkgId, std::list < std::pair <std::string, bool > > & list) const
{
#ifdef __READ_DB_IPC__
	std::unique_ptr <SocketClient> pSocketClient (new SocketClient(INTERFACE_NAME));

	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = pSocketClient->call("getAppPackagePrivacyInfo", pkgId, &result, &list);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

	return result;
#else
	return PrivacyDb::getInstance()->getAppPackagePrivacyInfo(pkgId, list);
#endif
}

int
PrivacyManagerClient::isUserPrompted(const std::string pkgId, bool& isPrompted) const
{
#ifdef __READ_DB_IPC__
	std::unique_ptr <SocketClient> pSocketClient (new SocketClient(INTERFACE_NAME));

	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = pSocketClient->call("isUserPrompted", pkgId, &result, &isPrompted);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

	return result;
#else
	return PrivacyDb::getInstance()->isUserPrompted(pkgId, isPrompted);
#endif
}

int
PrivacyManagerClient::setUserPrompted(const std::string pkgId, bool prompted)
{
	std::unique_ptr <SocketClient> pSocketClient (new SocketClient(INTERFACE_NAME));

	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = pSocketClient->call("setUserPrompted", pkgId, prompted, &result);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

	return result;
}

int
PrivacyManagerClient::getAppPackagesbyPrivacyId(const std::string privacyId, std::list < std::pair < std::string, bool > >& list) const
{
#ifdef __READ_DB_IPC__
	std::unique_ptr <SocketClient> pSocketClient (new SocketClient(INTERFACE_NAME));

	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = pSocketClient->call("getAppPackagesbyPrivacyId", privacyId, &result, &list);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "disconnect : %d", res);

	return result;
#else
	return PrivacyDb::getInstance()->getAppPackagesbyPrivacyId(privacyId, list);
#endif
}

int
PrivacyManagerClient::getPrivaycDisplayName(const std::string privacyId, std::string& displayName) const
{
	return PrivacyIdInfo::getPrivaycDisplayName(privacyId, displayName);
}

int
PrivacyManagerClient::getPrivaycDisplayNameStringId(const std::string privacyId, std::string& displayNameStringId) const
{
	return PrivacyIdInfo::getPrivaycDisplayNameStringId(privacyId, displayNameStringId);
}

int
PrivacyManagerClient::getPrivaycDescription(const std::string privacyId, std::string& description) const
{
	return PrivacyIdInfo::getPrivaycDescription(privacyId, description);
}

int
PrivacyManagerClient::getPrivaycDescriptionStringId(const std::string privacyId, std::string& descriptionStringId) const
{
	return PrivacyIdInfo::getPrivaycDescriptionStringId(privacyId, descriptionStringId);
}

int
PrivacyManagerClient::notifyUserNotConsented(const std::string pkgId, const std::string privacyId) const
{
	int result = PRIV_MGR_ERROR_SUCCESS;
	int res = m_pSocketClient->connect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);
	res = m_pSocketClient->call("notifyUserNotConsented", pkgId, privacyId, &result);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, m_pSocketClient->disconnect(), "call : %d", res);
	res = m_pSocketClient->disconnect();
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "connect : %d", res);

	return result;
}

int
PrivacyManagerClient::updatePackagePrivacyInfo(const std::string pkgId, const std::list<std::string> privilegeList, bool isServerOperation)
{
	std::list<std::string> privacyList;
	int res = PrivacyIdInfo::getPrivacyIdListFromPrivilegeList(privilegeList, privacyList);
	if (res != PRIV_MGR_ERROR_SUCCESS )
		return res;

	return PrivacyDb::getInstance()->updatePackagePrivacyInfo(pkgId, privacyList);
}
