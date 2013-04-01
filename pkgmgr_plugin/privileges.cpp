//
// Open Service Platform
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <errno.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <privacy_manager_client.h>
#include <dlog.h>
#include <list>
#include <string>

static const xmlChar _NODE_PRIVILEGES[]		= "privileges";
static const xmlChar _NODE_PRIVILEGE[]		= "privilege";


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

extern "C"
__attribute__ ((visibility("default")))
int PKGMGR_PARSER_PLUGIN_INSTALL(xmlDocPtr docPtr, const char* packageId)
{
	int ret;
	LOGI("enter");

	// Node: <privileges>
	xmlNodePtr curPtr = xmlFirstElementChild(xmlDocGetRootElement(docPtr));
	LOGD("Node: %s", curPtr->name);

	curPtr = curPtr->xmlChildrenNode;
	if (curPtr == NULL)
	{
		LOGD("No privileges");
		return 0;
	}

	std::list <std::string> privilegeList;
	while (curPtr != NULL)
	{
		LOGD("Node: %s", curPtr->name);

		if (xmlStrcmp(curPtr->name, _NODE_PRIVILEGE) == 0)
		{
			xmlChar* pPrivilege = xmlNodeListGetString(docPtr, curPtr->xmlChildrenNode, 1);
			LOGD(" value= %s", reinterpret_cast<char*>(pPrivilege));
			if (pPrivilege == NULL)
			{
				LOGE("Failed to get value");
				return -EINVAL;
			}
			privilegeList.push_back(std::string( reinterpret_cast<char*> (pPrivilege)));
		}
		curPtr = curPtr->next;
	}

	char** ppPrivilegeList = (char**) calloc(privilegeList.size() + 1, sizeof(char*));
	std::list <std::string>::iterator iter = privilegeList.begin();
	for (int i = 0; i < privilegeList.size(); ++i)
	{
		ppPrivilegeList[i] = (char*)calloc (strlen(iter->c_str()) + 1, sizeof(char));
		if (ppPrivilegeList[i] == NULL)
		{
			destroy_char_list(ppPrivilegeList, privilegeList.size() + 1);
			return -ENOMEM;
		}
		memcpy(ppPrivilegeList[i], iter->c_str(), strlen(iter->c_str()));
		++iter;
	}
	for (int i = 0; i < privilegeList.size(); ++i)
		LOGD(" values : %s", ppPrivilegeList[i]);

	ppPrivilegeList[privilegeList.size()] = (char*)calloc (2, sizeof(char));
	memcpy(ppPrivilegeList[privilegeList.size()], "\0", 1);

	ret = privacy_manager_client_install_privacy(packageId, (const char**) ppPrivilegeList);
	destroy_char_list(ppPrivilegeList, privilegeList.size() + 1);
	if (ret != PRIV_MGR_ERROR_SUCCESS)
	{
		LOGD("Failed to install privacy : %d", ret);
		return -EINVAL;
	}

	LOGI("leave");
    
    return 0;
}

extern "C"
__attribute__ ((visibility("default")))
int PKGMGR_PARSER_PLUGIN_UNINSTALL(xmlDocPtr docPtr, const char* packageId)
{
	LOGI("enter");

	privacy_manager_client_uninstall_privacy(packageId);

	return 0;
	LOGI("leave");	
}
