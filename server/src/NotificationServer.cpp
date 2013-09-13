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


#include <NotificationServer.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <PrivacyManagerTypes.h>
#include <Utils.h>
#include <gio/gio.h>

auto DBusConnectionDeleter = [&](DBusConnection* pPtr) { dbus_connection_close(pPtr); pPtr = NULL;};
const int MAX_LOCAL_BUF_SIZE = 128;

NotificationServer::NotificationServer(void)
	: m_initialized(false)
	, m_pDBusConnection(NULL)
{

}

NotificationServer::~NotificationServer(void)
{
	if (m_pDBusConnection)
	{
		g_object_unref(m_pDBusConnection);
		m_pDBusConnection = NULL;
	}
}

int
NotificationServer::initialize(void)
{
	if (m_initialized)
		return PRIV_MGR_ERROR_SUCCESS;
	
    GError* pGerror = NULL;
    g_type_init();

	m_pDBusConnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &pGerror);
	TryReturn(pGerror == NULL, PRIV_MGR_ERROR_SYSTEM_ERROR, g_error_free(pGerror), "g_dbus_get_sync : %s", pGerror->message);

	m_initialized = true;
	return PRIV_MGR_ERROR_SUCCESS;
}

int
NotificationServer::notifySettingChanged(const std::string pkgId, const std::string privacyId)
{
	if (!m_initialized)
		return PRIV_MGR_ERROR_INVALID_STATE;

    GError* pGerror = NULL;
	char* pPkgId = const_cast <char*> (pkgId.c_str());
	char* pPrivacyId = const_cast <char*> (privacyId.c_str());

    g_dbus_connection_emit_signal(m_pDBusConnection,
            NULL,
            DBUS_PATH.c_str(),
            DBUS_SIGNAL_INTERFACE.c_str(),
            DBUS_SIGNAL_SETTING_CHANGED.c_str(),
            g_variant_new("(ss)", pPkgId, pPrivacyId),
            &pGerror
        );
	TryReturn(pGerror == NULL, PRIV_MGR_ERROR_SYSTEM_ERROR, g_error_free(pGerror), "g_dbus_connection_emit_signal : %s", pGerror->message);

	return PRIV_MGR_ERROR_SUCCESS;
}

int
NotificationServer::notifyPkgRemoved(const std::string pkgId)
{
	if (!m_initialized)
		return PRIV_MGR_ERROR_INVALID_STATE;

    GError* pGerror = NULL;
	char* pPkgId = const_cast <char*> (pkgId.c_str());

    g_dbus_connection_emit_signal(m_pDBusConnection,
            NULL,
            DBUS_PATH.c_str(),
            DBUS_SIGNAL_INTERFACE.c_str(),
            DBUS_SIGNAL_PKG_REMOVED.c_str(),
            g_variant_new("(s)", pPkgId),
            &pGerror
        );
	TryReturn(pGerror == NULL, PRIV_MGR_ERROR_SYSTEM_ERROR, g_error_free(pGerror), "g_dbus_connection_emit_signal : %s", pGerror->message);

	return PRIV_MGR_ERROR_SUCCESS;
}
