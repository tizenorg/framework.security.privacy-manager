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

#ifndef __TIZEN_PRIVACYMGR_PRIVACY_MANAGER_CLIENT_TYPES_H
#define __TIZEN_PRIVACYMGR_PRIVACY_MANAGER_CLIENT_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN_API
#define EXTERN_API __attribute__((visibility("default")))
#endif

enum {
	PRIV_MGR_ERROR_SUCCESS = 0,

	PRIV_MGR_ERROR_NOT_INITIALIZED = -10,
	PRIV_MGR_ERROR_INVALID_PARAMETER = -11,
	PRIV_MGR_ERROR_OUT_OF_MEMORY = -12,
	PRIV_MGR_ERROR_IO_ERROR = -13,
	PRIV_MGR_ERROR_NO_DATA = -14,
	PRIV_MGR_ERROR_DB_ERROR = -15,
	PRIV_MGR_ERROR_IPC_ERROR = -16,
	PRIV_MGR_ERROR_INVALID_STATE = -17,
	PRIV_MGR_ERROR_SYSTEM_ERROR = -18,
	PRIV_MGR_ERROR_USER_NOT_CONSENTED = -19,

	PRIV_MGR_ERROR_UNKNOWN = -(0x99),
};


#ifdef __cplusplus
}
#endif

#endif //__TIZEN_PRIVACYMGR_PRIVACY_MANAGER_CLIENT_TYPES_H
