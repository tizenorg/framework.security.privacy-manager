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

#ifndef _PRIVACY_CHECKER_H_
#define _PRIVACY_CHECKER_H_

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <PrivacyManagerTypes.h>
#include <dbus/dbus.h>
#include <glib.h>

struct sqlite3;

class EXTERN_API PrivacyChecker
{
private:
	static std::map < std::string, bool > m_privacyCache;
	static std::map < std::string, std::map < std::string, bool > > m_privacyInfoCache;
	static std::string m_pkgId;
	static bool m_isInitialized;
	static std::mutex m_cacheMutex;
	static DBusConnection* m_pDBusConnection;
	static GMainLoop* m_pLoop;
	static GMainContext* m_pHandlerGMainContext;
	static pthread_t m_signalThread;
	static pthread_mutex_t syncMutex;
	static pthread_cond_t syncCondition;


private:
	static int initializeDbus(void);
	static int finalizeDbus(void);
	static int updateCache(const std::string pkgId, std::string privacyId, std::map < std::string, bool >& pkgCacheMap);
	static int updateCache(const std::string pkgId, std::map < std::string, bool >& pkgCacheMap);
	static void printCache(void);
	static void* runSignalListenerThread(void* pData);
	static int getCurrentPkgId(std::string& pkgId);
	static int check(const std::string privacyId, std::map < std::string, bool >& privacyMap);

public:
	// for Checking in App Process
	static int initialize(const std::string pkgId);
	static int check(const std::string pkgId, const std::string privacyId);
	static int checkWithPrivilege(const std::string pkgId, const std::string privilegeId);
	static int checkWithDeviceCap(const std::string pkgId, const std::string deviceCap);
	
	// for Checking in Server Process
	static int initialize(void);
	static int check(const std::string privacyId);
	static int checkWithPrivilege(const std::string privilegeId);
	static int checkWithDeviceCap(const std::string deviceCap);
	
	// common
	static int finalize(void);
	static DBusHandlerResult handleNotification(DBusConnection* connection, DBusMessage* message, void* user_data);
};

#endif // _PRIVACY_CHECKER_H_