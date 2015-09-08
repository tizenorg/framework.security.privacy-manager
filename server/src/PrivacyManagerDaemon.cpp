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

#include <string>
#include <PrivacyManagerDaemon.h>
#include <PrivacyInfoService.h>
#include <PrivacyManagerServer.h>
#include <SocketService.h>

PrivacyManagerDaemon* PrivacyManagerDaemon::pInstance = NULL;

PrivacyManagerDaemon::PrivacyManagerDaemon(void)
	: pSocketService(NULL)
{

}

PrivacyManagerDaemon::~PrivacyManagerDaemon(void)
{

}

PrivacyManagerDaemon*
PrivacyManagerDaemon::getInstance(void)
{
	if (pInstance == NULL)
		pInstance = new PrivacyManagerDaemon();
	PrivacyManagerServer::getInstance();
	return pInstance;
}

int
PrivacyManagerDaemon::initialize(void)
{
	if (pSocketService == NULL)
		pSocketService = new SocketService();

	pSocketService->initialize();

	PrivacyInfoService::registerCallbacks(pSocketService);

	return 0;
}

int
PrivacyManagerDaemon::start(void)
{
	if (pSocketService == NULL)
		return -1;
	return pSocketService->start();

	return 0;
}

int
PrivacyManagerDaemon::stop(void)
{
	pSocketService->stop();
	return 0;
}

int
PrivacyManagerDaemon::shutdown(void)
{
	pSocketService->shutdown();
	return 0;
}
