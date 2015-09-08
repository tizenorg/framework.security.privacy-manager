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

#include <sstream>
#include <dlog.h>
#include <Utils.h>
#include <PrivacyIdInfo.h>
#include <PrivacyManagerServer.h>
#include <PrivacyManagerTypes.h>
#include <PrivacyDb.h>
#include <privilege-control.h>
#include <security-server.h>
#include <pkgmgr-info.h>
#include <libintl.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <vconf.h>


std::mutex PrivacyManagerServer::m_singletonMutex;
PrivacyManagerServer* PrivacyManagerServer::m_pInstance = NULL;
const char *pLocationPrivacy = "http://tizen.org/privacy/location";
const char *pLocationPrivilege = "http://tizen.org/privilege/location";
pkgmgrinfo_client *__pc = NULL;
static const char *VCONF_KEY = "db/menu_widget/language";

static int pkgmgrInfoCallBack(int req_id, const char *pkg_type,
							const char *pkgid, const char *key, const char *val,
							const void *pmsg, void *user_data)
{
	if(strncmp(pkg_type, "wgt", 3) == 0 && strncmp(key, "end", 3) == 0 )
	{
		LOGD("pkg_id : %s, pkg_type : %s will remove from DB", pkgid, pkg_type);
		int res = PrivacyDb::getInstance()->removeAppPackagePrivacyInfo(pkgid);
		if(res != PRIV_MGR_ERROR_SUCCESS)
			LOGE("failed to remove data : %d", res);
	}
	return 0;
}

static void languageCallBack(keynode_t *node, void* user_data)
{
	char *language_set = vconf_get_str(VCONF_KEY);
	if(!language_set)
	{
		LOGE("Failed to get laguage set");
	}
	char* result = setlocale(LC_ALL, language_set);
	if(!result)
	{
		LOGE("Failed to set locale");
	}

	if (language_set)
		free(language_set);
}

void
PrivacyManagerServer::createDB(void)
{

}

int
PrivacyManagerServer::setPrivacySetting(const std::string pkgId, const std::string privacyId, bool enabled)
{
	int res = PrivacyDb::getInstance()->setPrivacySetting(pkgId, privacyId, enabled);
	TryReturn( res == PRIV_MGR_ERROR_SUCCESS, res, , "privacyDb::setPrivacySetting : %d", res);

	res = m_notificationServer.notifySettingChanged(pkgId, privacyId);
	TryReturn( res == PRIV_MGR_ERROR_SUCCESS, res, , "NotificationServer::notifySettingChanged : %d", res);

	res = setPermissions(pkgId, privacyId, enabled);
	TryReturn( res == PRIV_MGR_ERROR_SUCCESS, res, , "PrivacyManagerServer::setPermissions : %d", res);

	return res;
}

int
PrivacyManagerServer::getPrivacyAppPackages(std::list <std::string>& list)
{
	return PrivacyDb::getInstance()->getPrivacyAppPackages(list);
}

int
PrivacyManagerServer::getAppPackagePrivacyInfo(const std::string pkgId, std::list < std::pair < std::string, bool > >& privacyInfoList)
{
	return PrivacyDb::getInstance()->getAppPackagePrivacyInfo(pkgId, privacyInfoList);
}


int
PrivacyManagerServer::addAppPackagePrivacyInfo(const std::string pkgId, const std::list < std::string > privilegeList, bool privacyPopupRequired)
{
	return PrivacyDb::getInstance()->addAppPackagePrivacyInfo(pkgId, privilegeList, privacyPopupRequired);
}

int
PrivacyManagerServer::removeAppPackagePrivacyInfo(const std::string pkgId)
{
	int res = PrivacyDb::getInstance()->removeAppPackagePrivacyInfo(pkgId);
	TryReturn( res == PRIV_MGR_ERROR_SUCCESS, res, , "privacyDb::removeAppPackagePrivacyInfo : %d", res);

	res = m_notificationServer.notifyPkgRemoved(pkgId);
	TryReturn( res == PRIV_MGR_ERROR_SUCCESS, res, , "NotificationServer::notifyPkgRemoved : %d", res);

	return res;
}

int
PrivacyManagerServer::isUserPrompted(const std::string pkgId, bool& isPrompted)
{
	return PrivacyDb::getInstance()->isUserPrompted(pkgId, isPrompted);
}

int
PrivacyManagerServer::setUserPrompted(const std::string pkgId, bool prompted)
{
	return PrivacyDb::getInstance()->setUserPrompted(pkgId, prompted);
}

PrivacyManagerServer::PrivacyManagerServer(void)
{
	// for non privileged W3C location apps
	int event_type = PMINFO_CLIENT_STATUS_UNINSTALL;
	__pc = pkgmgrinfo_client_new(PMINFO_LISTENING);
	pkgmgrinfo_client_set_status_type(__pc, event_type);
	pkgmgrinfo_client_listen_status(__pc, pkgmgrInfoCallBack, NULL);

	// set locale for notification
	if(vconf_notify_key_changed(VCONF_KEY, languageCallBack, NULL) < 0)
		LOGE("Failed to register vconf callback");
	else
		LOGD("Succeed to register vconf vconf callback");

	char *language_set = vconf_get_str(VCONF_KEY);
	setlocale(LC_ALL, language_set);
	if (language_set)
		free(language_set);
}

PrivacyManagerServer::~PrivacyManagerServer(void)
{
	if(__pc)
	{
		pkgmgrinfo_client_free(__pc);
	}
}

PrivacyManagerServer*
PrivacyManagerServer::getInstance(void)
{
	std::lock_guard < std::mutex > guard(m_singletonMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new PrivacyManagerServer();
		
		m_pInstance->m_notificationServer.initialize();
	}

	return m_pInstance;
}

void destroy_char_list(char** ppList, int size)
{
	int i;
	for (i = 0; i < size; ++i)
	{
		if (ppList[i])
			free(ppList[i]);
	}
	free(ppList);
}

int privilegeListCallback(const char *privilege_name, void *user_data)
{
	if (user_data == NULL)
		return PRIV_MGR_ERROR_SYSTEM_ERROR;

	std::list <std::string>* pPrivilegList = (std::list <std::string>*)user_data;
	pPrivilegList->push_back(std::string(privilege_name));
	return 0;
}

int
PrivacyManagerServer::setPermissions(const std::string pkgId, const std::string privacyId, bool enabled)
{
	SECURE_LOGD("pkgId = %s / privacyId = %s, enabled = %d", pkgId.c_str(), privacyId.c_str(), enabled);

	int ret = PMINFO_R_OK;
	int res = PRIV_MGR_ERROR_SUCCESS;

	char *pType = NULL;
	app_type_t appType = APP_TYPE_OSP;

	pkgmgrinfo_pkginfo_h handle;
	ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgId.c_str(), &handle);
	TryReturn(ret == PMINFO_R_OK, PRIV_MGR_ERROR_SYSTEM_ERROR,, "pkgmgrinfo_pkginfo_get_pkginfo was failed : %d", ret);

	ret = pkgmgrinfo_pkginfo_get_type(handle, &pType);
	TryReturn(ret == PMINFO_R_OK, PRIV_MGR_ERROR_SYSTEM_ERROR, pkgmgrinfo_pkginfo_destroy_pkginfo(handle), "pkgmgrinfo_pkginfo_get_type was failed :%d", ret);

	int typeSize = sizeof(pType);
	if (strncmp(pType, "wgt", typeSize) == 0)
	{
		appType = APP_TYPE_WGT;
	}
	else if (strncmp(pType, "tpk", typeSize) == 0)
	{
		appType = APP_TYPE_OSP;
	}
	else if (strncmp(pType, "rpm", typeSize) == 0)
	{
		appType = APP_TYPE_EFL;
	}
	else
	{
		LOGE("Type of package is incorrect. [TYPE: %s]", pType);
		pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
		return PRIV_MGR_ERROR_SYSTEM_ERROR;
	}

	std::list <std::string> pkgPrivilegeList;
	res = pkgmgrinfo_pkginfo_foreach_privilege(handle, privilegeListCallback, &pkgPrivilegeList);
	TryReturn(ret == PMINFO_R_OK, PRIV_MGR_ERROR_SYSTEM_ERROR, pkgmgrinfo_pkginfo_destroy_pkginfo(handle), "pkgmgrinfo_pkginfo_foreach_privilege was failed :%d", ret);
	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);

	std::list <std::string> privacyPrivilegeList;
	res = PrivacyIdInfo::getPrivilegeListFromPrivacyId(privacyId, privacyPrivilegeList);
	TryReturn(res == PRIV_MGR_ERROR_SUCCESS, PRIV_MGR_ERROR_SYSTEM_ERROR, pkgPrivilegeList.clear(), "getPrivilegeListFromPrivacyId was failed.");

	std::list <std::string> privilegeList;
	for (std::list <std::string>::iterator privacyPrivilegeListIter = privacyPrivilegeList.begin(); privacyPrivilegeListIter != privacyPrivilegeList.end(); ++privacyPrivilegeListIter)
	{
		for (std::list <std::string>::iterator pkgPrivilegeListIter = pkgPrivilegeList.begin(); pkgPrivilegeListIter != pkgPrivilegeList.end(); ++pkgPrivilegeListIter)
		{
			if ((*privacyPrivilegeListIter).compare(*pkgPrivilegeListIter) == 0)
			{
				privilegeList.push_back(std::string(*privacyPrivilegeListIter));
				SECURE_LOGD("smack rule control [%s]", (*privacyPrivilegeListIter).c_str());
				break;
			}
		}
	}

	// for W3C Web Application using location API
	if(appType == APP_TYPE_WGT && strncmp(privacyId.c_str(), pLocationPrivacy, strlen(pLocationPrivacy)) == 0)
	{
		LOGD("WGT location privacy request for w3c location api");
		std::list<std::string>::iterator iter = find(privilegeList.begin(), privilegeList.end(), std::string(pLocationPrivilege));

		if(iter == privilegeList.end())
			privilegeList.push_back(std::string(pLocationPrivilege));
	}

	privacyPrivilegeList.clear();
	pkgPrivilegeList.clear();

	unsigned int listSize = privilegeList.size();
	char** ppPrivilegeList = (char**) calloc(listSize + 1, sizeof(char*));
	TryReturn(ppPrivilegeList != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, privilegeList.clear(),"calloc was failed.");

	std::list <std::string>::iterator iter = privilegeList.begin();
	for (unsigned int i = 0; i < listSize; ++i)
	{
		ppPrivilegeList[i] = (char*)calloc (strlen(iter->c_str()) + 1, sizeof(char));
		if (ppPrivilegeList[i] == NULL)
		{
			destroy_char_list(ppPrivilegeList, listSize + 1);
			privilegeList.clear();
			LOGE("calloc was failed.");
			return PRIV_MGR_ERROR_OUT_OF_MEMORY;
		}
		memcpy(ppPrivilegeList[i], iter->c_str(), strlen(iter->c_str()));
		++iter;
	}
	privilegeList.clear();

	ppPrivilegeList[listSize] = NULL;
	if (enabled == true)
	{
		LOGD("call: security_server_app_enable_permissions()");
		res = security_server_app_enable_permissions(pkgId.c_str(), appType, (const char**) ppPrivilegeList, 1);
		LOGD("leave: security_server_app_enable_permissions()");
		TryReturn(res == 0/*SECURITY_SERVER_SUCCESS*/, PRIV_MGR_ERROR_SYSTEM_ERROR, destroy_char_list(ppPrivilegeList, listSize + 1), "security_server_app_enable_permissions was failed : %d", res);
	}
	else
	{
		LOGD("call: security_server_app_disable_permissions()");
		res = security_server_app_disable_permissions(pkgId.c_str(), appType, (const char**) ppPrivilegeList);
		LOGD("leave: security_server_app_disable_permissions()");
		TryReturn(res == 0/*SECURITY_SERVER_SUCCESS*/, PRIV_MGR_ERROR_SYSTEM_ERROR, destroy_char_list(ppPrivilegeList, listSize + 1), "security_server_app_disable_permissions was failed : %d", res);
	}
	destroy_char_list(ppPrivilegeList, listSize + 1);
	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyManagerServer::notifyUserNotConsented(const std::string pkgId, const std::string privacyId)
{
	int res = PRIV_MGR_ERROR_SUCCESS;

	const char* notifyStringFormat = dgettext("privilege", "IDS_ST_TPOP_P1SS_DISABLED_IN_P2SS_ABB");
	char notifyString[1024] = {0,};

	bool packageNameFlag = true;
	char* packageName = NULL;
	pkgmgrinfo_pkginfo_h handle;
    res = pkgmgrinfo_pkginfo_get_pkginfo(pkgId.c_str(), &handle);
    if (res != PMINFO_R_OK)
    {
		SECURE_LOGE("pkgmgrinfo_pkginfo_get_pkginfo() was failed: result[%d]", res);
		packageNameFlag = false;
    }
    else
    {
    	res = pkgmgrinfo_pkginfo_get_label(handle, &packageName);
		if (res != PMINFO_R_OK)
		{
			SECURE_LOGE("pkgmgrinfo_pkginfo_get_label() was failed: result[%d]", res);
			packageNameFlag = false;
		}
    }

	std::string privacyDisplayName;
	res = PrivacyIdInfo::getPrivaycDisplayName(privacyId, privacyDisplayName);
	if (res != PRIV_MGR_ERROR_SUCCESS)
	{
		privacyDisplayName = privacyId;
	}

	if (packageNameFlag)
	{
		snprintf(notifyString, 1024, notifyStringFormat, privacyDisplayName.c_str(), packageName);
	}
	else
	{
		snprintf(notifyString, 1024, notifyStringFormat, privacyDisplayName.c_str(), pkgId.c_str());
	}

	void* so_handle = dlopen("libnotification.so", RTLD_LAZY | RTLD_GLOBAL);
	TryReturn(so_handle != NULL, PRIV_MGR_ERROR_SYSTEM_ERROR, , "Failed to open notification binary");

	int(*DoNotificationFunc)(const char*) = NULL;

	char* errormsg = NULL;
	DoNotificationFunc = reinterpret_cast<int(*)(const char*)>(dlsym(so_handle, "notification_status_message_post"));
	errormsg = dlerror();
	TryReturn(errormsg == NULL, PRIV_MGR_ERROR_SYSTEM_ERROR, dlclose(so_handle), "Failed to find symbol");

	DoNotificationFunc(notifyString);
	dlclose(so_handle);
	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
	return PRIV_MGR_ERROR_SUCCESS;
}

