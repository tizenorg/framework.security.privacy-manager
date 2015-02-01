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

#ifndef SECURITYSOCKETCLIENT_H_
#define SECURITYSOCKETCLIENT_H_

#include <memory>
#include <string>
#include <dlog.h>
#include "SocketConnection.h"

/* IMPORTANT:
 * Methods connect(), call() and disconnected() should be called one by one.
 * Between connect() and disconnect() you can use call() only once.
 * It is because of timeout on call, e.g. to avoid waiting for corrupted data.
 */

/* USAGE:
 * Class should be used according to this scheme:
 * SocketClient client("Interface Name");
 * (...)
 * client.connect();
 * client.call("Method name", in_arg1, in_arg2, ..., in_argN,
 *             out_arg1, out_arg2, ..., out_argM);
 * client.disconnect();
 * (...)
 *
 * input parameters of the call are passed with reference,
 * output ones are passed as pointers - parameters MUST be passed this way.
 *
 * Currently client supports serialization and deserialization of simple types
 * (int, char, float, unsigned), strings (std::string and char*) and
 * some STL containers (std::vector, std::list, std::map, std::pair).
 * Structures and classes are not (yet) supported.
 */

#include <string>
#include <PrivacyManagerTypes.h>

class EXTERN_API SocketClient
{
public:

	SocketClient(const std::string &interfaceName);
	int connect();
	int disconnect();

	int call(std::string methodName)
	{
		int res = make_call(m_interfaceName);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		res = make_call(methodName);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename ...Args>
	int call(std::string methodName, const Args&... args)
	{
		int res = make_call(m_interfaceName);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);
		res = make_call(methodName);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);
		res = make_call(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int read(T* outvalue)
	{
		return m_socketConnector->read(outvalue);
	}
private:
	template<typename T, typename ...Args>
	int make_call(const T& invalue, const Args&... args)
	{
		int res = make_call(invalue);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);
		res = make_call(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int make_call(const T& invalue)
	{
		return m_socketConnector->write(invalue);
	}

	template<typename T, typename ...Args>
	int make_call(const T* invalue, const Args&... args)
	{
		int res = make_call(invalue);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);
		res = make_call(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int make_call(const T* invalue)
	{
		return m_socketConnector->write(invalue);
	}

	template<typename T, typename ...Args>
	int make_call(T * outvalue, const Args&... args)
	{
		int res = make_call(outvalue);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);
		res = make_call(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "make_call : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int make_call(T* outvalue)
	{
		return m_socketConnector->read(outvalue);
    }


private:
	std::string m_serverAddress;
	std::string m_interfaceName;
	std::unique_ptr<SocketConnection> m_socketConnector;
	int m_socketFd;
};

#endif /* SECURITYSOCKETCLIENT_H_ */