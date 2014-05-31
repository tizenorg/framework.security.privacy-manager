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

#ifndef _PRIVACY_INFO_SERVICE_H_
#define _PRIVACY_INFO_SERVICE_H_

#include <SocketConnection.h>
#include <SocketService.h>

class PrivacyInfoService {
private:
	inline static std::string getInterfaceName(void)
	{
		return "PrivacyInfoService";
	}

public:
	static void registerCallbacks(SocketService* pSocketService)
	{
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("addPrivacyInfo"), addPrivacyInfo);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("removePrivacyInfo"), removePrivacyInfo);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("setPrivacySetting"), setPrivacySetting);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("getPrivacyAppPackages"), getPrivacyAppPackages);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("getAppPackagePrivacyInfo"), getAppPackagePrivacyInfo);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("isUserPrompted"), isUserPrompted);
		pSocketService->registerServiceCallback(getInterfaceName(), std::string("setUserPrompted"), setUserPrompted);
	}

	static void addPrivacyInfo(SocketConnection* pConnector);

	static void removePrivacyInfo(SocketConnection* pConnector);

	static void setPrivacySetting(SocketConnection* pConnector);
	
	// input message format
	// interface_name(str) | method_name(str)
	// output message format
	// result (int) | list size (int) | serialized package list (using delimeter)
	static void getPrivacyAppPackages(SocketConnection* pConnector);

	static void getAppPackagePrivacyInfo(SocketConnection* pConnector);

	static void isUserPrompted(SocketConnection* pConnector);
	
	static void setUserPrompted(SocketConnection* pConnector);

};
#endif // _PRIVACY_INFO_SERVICE_H_