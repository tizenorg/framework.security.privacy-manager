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

#ifndef _PRIVACY_MANAGER_TYPES_H_
#define _PRIVACY_MANAGER_TYPES_H_

#include <string>
#include <privacy_manager_client_types.h>

static const std::string PRIVACY_DB_PATH("/opt/dbspace/.privacy.db");
static const std::string PRIVACY_INFO_DB_PATH("/opt/dbspace/.privacylist.db");
static const std::string SERVER_ADDRESS ("/tmp/privacy_manager_server");
static const std::string DBUS_PATH("/privacy_manager/dbus_notification");
static const std::string DBUS_SIGNAL_INTERFACE("org.tizen.privacy_manager.signal");
static const std::string DBUS_SIGNAL_SETTING_CHANGED("privacy_setting_changed");
static const std::string DBUS_SIGNAL_PKG_REMOVED("privacy_pkg_removed");

#endif
