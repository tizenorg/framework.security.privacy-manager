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

#include <privacy_manager_daemon.h>
#include <PrivacyManagerDaemon.h>

int privacy_manager_daemon_initialize(void)
{
	PrivacyManagerDaemon* pPrivacyDaemon = PrivacyManagerDaemon::getInstance();

	return pPrivacyDaemon->initialize();
}

int privacy_manager_daemon_start(void)
{
	PrivacyManagerDaemon* pInstance = PrivacyManagerDaemon::getInstance();

	return pInstance->start();
}

int privacy_manager_daemon_stop(void)
{
	PrivacyManagerDaemon* pInstance = PrivacyManagerDaemon::getInstance();

	return pInstance->stop();
}

int privacy_manager_daemon_shutdown(void)
{
	PrivacyManagerDaemon* pInstance = PrivacyManagerDaemon::getInstance();

	return pInstance->shutdown();
}
