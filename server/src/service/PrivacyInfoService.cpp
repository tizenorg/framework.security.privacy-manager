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

#include <PrivacyInfoService.h>
#include <PrivacyManagerServer.h>
#include <dlog.h>
#include <Utils.h>
#include <security-server.h>

void
PrivacyInfoService::addPrivacyInfo(SocketConnection* pConnector)
{
	std::string pkgId;
	std::list < std::string > list;
	bool privacyPopupRequired = true;
	pConnector->read(&pkgId, &list, &privacyPopupRequired);

	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();
	int result = pPrivacyManagerServer->addAppPackagePrivacyInfo(pkgId, list, privacyPopupRequired);

	pConnector->write(result);
}

void
PrivacyInfoService::removePrivacyInfo(SocketConnection* pConnector)
{
	std::string pkgId;
	pConnector->read(&pkgId);

	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();
	int res = pPrivacyManagerServer->removeAppPackagePrivacyInfo(pkgId);

	pConnector->write(res);
}

void
PrivacyInfoService::setPrivacySetting(SocketConnection* pConnector)
{
	std::string pkgId;
	std::string privacyId;
	bool enabled = false;
	pConnector->read(&pkgId, &privacyId, &enabled);

	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();
	int result = pPrivacyManagerServer->setPrivacySetting(pkgId, privacyId, enabled);

	pConnector->write(result);
}

void
PrivacyInfoService::getPrivacyAppPackages(SocketConnection* pConnector)
{
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();
	std::list <std::string> list;
	int result = pPrivacyManagerServer->getPrivacyAppPackages(list);

	pConnector->write( (unsigned int) result);
	pConnector->write( (unsigned int) list.size());
	pConnector->write(list);
}

void
PrivacyInfoService::getAppPackagePrivacyInfo(SocketConnection* pConnector)
{
	std::string pkgId;

	pConnector->read(&pkgId);
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();

	std::list < std::pair < std::string, bool > > infoList;

	int res = pPrivacyManagerServer->getAppPackagePrivacyInfo(pkgId, infoList);

	pConnector->write( res );
	pConnector->write( infoList );
}

void
PrivacyInfoService::getAppPackagesbyPrivacyId(SocketConnection* pConnector)
{
	std::string privacyId;

	pConnector->read(&privacyId);
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();

	
	std::list < std::pair < std::string, bool > > pkgList;
	
	int res = pPrivacyManagerServer->getAppPackagesbyPrivacyId(privacyId, pkgList);

	pConnector->write( res );
	pConnector->write( pkgList );
}

void
PrivacyInfoService::isUserPrompted(SocketConnection* pConnector)
{
	std::string pkgId;

	pConnector->read(&pkgId);
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();

	bool isPrompted;
	int res = pPrivacyManagerServer->isUserPrompted(pkgId, isPrompted);

	pConnector->write( res );
	pConnector->write( isPrompted );

}

void
PrivacyInfoService::setUserPrompted(SocketConnection* pConnector)
{
	std::string pkgId;
	bool prompted = false;

	pConnector->read(&pkgId, &prompted);
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();

	int res = pPrivacyManagerServer->setUserPrompted(pkgId, prompted);
	LOGI("write");
	pConnector->write( res );

}

void
PrivacyInfoService::notifyUserNotConsented(SocketConnection* pConnector)
{
	std::string pkgId, privacyId;

	pConnector->read(&pkgId, &privacyId);
	PrivacyManagerServer* pPrivacyManagerServer = PrivacyManagerServer::getInstance();

	int res = pPrivacyManagerServer->notifyUserNotConsented(pkgId, privacyId);
	pConnector->write( res );

}

int
PrivacyInfoService::checkPrivacySettingPermission(int fd)
{
	int res = PRIV_MGR_ERROR_SUCCESS;
	int retval = security_server_check_privilege_by_sockfd(fd, "privacy-manager::write", "w");
	if(retval != SECURITY_SERVER_API_SUCCESS)
	{
		LOGE("This application does not have permission to read privacy");
		return PRIV_MGR_ERROR_PERMISSION_DENIED;
	}

	return res;
}

int
PrivacyInfoService::checkPrivacyReadPermission(int fd)
{
	int retval = security_server_check_privilege_by_sockfd(fd, "privacy-manager::read", "r");
	if(retval != SECURITY_SERVER_API_SUCCESS)
	{
		LOGE("This application does not have permission to read privacy");
		return PRIV_MGR_ERROR_PERMISSION_DENIED;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}
