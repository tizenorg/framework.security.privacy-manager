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

#include <PrivacyIdInfo.h>
#include <privacy_manager_client_types.h>
#include <PrivacyManagerTypes.h>
#include <dlog.h>
#include <set>
#include <Utils.h>
#include <libintl.h>

std::map <std::string, std::string> PrivacyIdInfo::m_privilegeToPrivacyMap;
bool PrivacyIdInfo:: m_isInitialized;

int
PrivacyIdInfo::initialize(void)
{
	static const std::string sqlPrivilege("SELECT PRIVILEGE_ID, PRIVACY_ID from PrivilegeToPrivacyTable");

	openDb(PRIVACY_INFO_DB_PATH.c_str(), pDbHandler, SQLITE_OPEN_READONLY);
	prepareDb(pDbHandler, sqlPrivilege.c_str(), pStmtPrivilege);

	int res;
	while ( (res = sqlite3_step(pStmtPrivilege.get())) == SQLITE_ROW )
	{
		const char* privilegeId =  reinterpret_cast < const char* > (sqlite3_column_text(pStmtPrivilege.get(), 0));
		const char* privacyId =  reinterpret_cast < const char* > (sqlite3_column_text(pStmtPrivilege.get(), 1));
		m_privilegeToPrivacyMap.insert(std::map < std::string, std::string >::value_type(std::string(privilegeId), std::string(privacyId)));
	}

	m_isInitialized = true;

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getPrivacyIdFromPrivilege(const std::string privilege, std::string& privacyId)
{
	if (!m_isInitialized)
		initialize();
	std::map < std::string, std::string >::iterator iter = m_privilegeToPrivacyMap.find(privilege);
	if (iter == m_privilegeToPrivacyMap.end())
		return PRIV_MGR_ERROR_NO_DATA;
	privacyId = iter->second;

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getPrivilegeListFromPrivacyId(const std::string privacyId, std::list < std::string> & privilegeList)
{
	if (!m_isInitialized)
		initialize();

	privilegeList.clear();
	for (std::map < std::string, std::string >::iterator iter = m_privilegeToPrivacyMap.begin(); iter != m_privilegeToPrivacyMap.end(); ++iter)
	{
		if (privacyId.compare((iter->second)) == 0)
		{
			privilegeList.push_back(iter->first);
		}
	}

	if (privilegeList.size() == 0)
	{
		LOGE("PrivilegeList of %s privacy is empty!", privacyId.c_str());
		return PRIV_MGR_ERROR_NO_DATA;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getPrivacyIdListFromPrivilegeList(const std::list < std::string> privilegeList, std::list < std::string> & privacyIdList)
{
	if (!m_isInitialized)
		initialize();

	privacyIdList.clear();

	std::set <std::string> privacyIdSet;

	for (std::list < std::string > ::const_iterator iter = privilegeList.begin(); iter != privilegeList.end(); ++iter)
	{
		std::string privacyId;
		int res = getPrivacyIdFromPrivilege(*iter, privacyId);
		if (res == PRIV_MGR_ERROR_SUCCESS)
			privacyIdSet.insert(privacyId);
	}
	
	for (std::set < std::string >::iterator iter = privacyIdSet.begin(); iter != privacyIdSet.end(); ++iter)
	{
		privacyIdList.push_back(*iter);
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getAllPrivacyId(std::list < std::string >& privacyIdList)
{
	static const std::string sql("SELECT PRIVACY_ID from PrivacyInfo");

	if (!m_isInitialized)
		initialize();
	
	openDb(PRIVACY_INFO_DB_PATH.c_str(), pDbHandler, SQLITE_OPEN_READONLY);
	prepareDb(pDbHandler, sql.c_str(), pStmt);

	int res;
	while ( (res = sqlite3_step(pStmt.get())) == SQLITE_ROW )
	{
		const char* privacyId =  reinterpret_cast < const char* > (sqlite3_column_text(pStmt.get(), 0));
		privacyIdList.push_back(std::string(privacyId));
		SECURE_LOGD(" privacy Id : %s", privacyId);
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getPrivaycDisplayName(const std::string privacyId, std::string& displayName)
{
	if (!m_isInitialized)
		initialize();

	std::string sql = std::string("SELECT STR_MODULE_ID, STR_NAME_ID from PrivacyInfo where PRIVACY_ID=?");

	openDb(PRIVACY_INFO_DB_PATH.c_str(), pDbHandler, SQLITE_OPEN_READONLY);
	prepareDb(pDbHandler, sql.c_str(), pStmt);

	int res = sqlite3_bind_text(pStmt.get(), 1, privacyId.c_str(), -1, SQLITE_TRANSIENT);
	TryReturn( res == SQLITE_OK, PRIV_MGR_ERROR_DB_ERROR, , "sqlite3_bind_text : %d", res);

	if ( sqlite3_step(pStmt.get()) == SQLITE_ROW )
	{
		const char* pModuleId = reinterpret_cast < const char* > (sqlite3_column_text(pStmt.get(), 0));
		const char* pNameId = reinterpret_cast < const char* > (sqlite3_column_text(pStmt.get(), 1));

		if (pNameId == NULL)
			displayName = privacyId;
		else
			displayName = std::string(dgettext(pModuleId, pNameId));
	}
	else
	{
		LOGI("Cannot find privacy string %s ", privacyId.c_str());
		return PRIV_MGR_ERROR_NO_DATA;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}

int
PrivacyIdInfo::getPrivaycDescription(const std::string privacyId, std::string& displayName)
{
	if (!m_isInitialized)
		initialize();

	std::string sql = std::string("SELECT STR_MODULE_ID, STR_NAME_ID from PrivacyInfo where PRIVACY_ID=?");

	openDb(PRIVACY_INFO_DB_PATH.c_str(), pDbHandler, SQLITE_OPEN_READONLY);
	prepareDb(pDbHandler, sql.c_str(), pStmt);

	int res = sqlite3_bind_text(pStmt.get(), 1, privacyId.c_str(), -1, SQLITE_TRANSIENT);
	TryReturn( res == SQLITE_OK, PRIV_MGR_ERROR_DB_ERROR, , "sqlite3_bind_text : %d", res);

	if ( sqlite3_step(pStmt.get()) == SQLITE_ROW )
	{
		const char* pModuleId = reinterpret_cast < const char* > (sqlite3_column_text(pStmt.get(), 0));
		const char* pNameId = reinterpret_cast < const char* > (sqlite3_column_text(pStmt.get(), 0));

		displayName = std::string(dgettext(pModuleId, pNameId));
	}
	else
	{
		LOGI("Cannot find privacy string %s ", privacyId.c_str());
		return PRIV_MGR_ERROR_NO_DATA;
	}

	return PRIV_MGR_ERROR_SUCCESS;
}
