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

#ifndef _SOCKET_CONNECTION_H_
#define _SOCKET_CONNECTION_H_

#include <dlog.h>
#include <new>
#include <list>
#include <utility>
#include <SocketStream.h>
#include <PrivacyManagerTypes.h>
#include <iostream>
#include <Utils.h>
/*
 * This class implements interface for generic read and write from given socket.
 * It does not maintain socket descriptor, so any connecting and disconnecting should be
 */

/*
 * Throws SocketConnectionException when read/write will not succeed or if any bad allocation
 * exception occurs during read.
 */

class EXTERN_API SocketConnection
{

public:

	explicit SocketConnection(int socket_fd) : m_socketStream(socket_fd){
//		LOGI("Created");
	}

	template<typename T, typename ...Args>
	int read(T* out, const Args&... args )
	{
		int res = read(out);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);
		res = read(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int read(T* out)
	{
		int length = 0;
		int res = m_socketStream.readStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "readStream : %d", res);
		char* pBuf = new (std::nothrow) char[length + 1];
		TryReturn(pBuf != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, , "new : %d", PRIV_MGR_ERROR_OUT_OF_MEMORY);

		res = m_socketStream.readStream(length, pBuf);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, delete[] pBuf, "readStream : %d", res);

		pBuf[length] = 0;

		out = T(pBuf);

		delete[] pBuf;

		return PRIV_MGR_ERROR_SUCCESS;
	}
	
	int read(bool* pB)
	{
		int length = 0;
		int res = m_socketStream.readStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "readStream : %d", res);

		char* pBuf = new (std::nothrow) char[length + 1];
		TryReturn(pBuf != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, , "new : %d", PRIV_MGR_ERROR_OUT_OF_MEMORY);

		res = m_socketStream.readStream(length, pBuf);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, delete[] pBuf, "readStream : %d", res);

		pBuf[length] = 0;

		*pB = * reinterpret_cast <bool* > (pBuf);

		delete[] pBuf;

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int read(bool& b)
	{
		return read(&b);
	}
	
	int read(int& i)
	{
		return read(&i);
	}

	int read(int* pI)
	{
		int length = 0;
		int res = m_socketStream.readStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "readStream : %d", res);

		char* pBuf = new (std::nothrow) char[length + 1];
		TryReturn(pBuf != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, , "new : %d", PRIV_MGR_ERROR_OUT_OF_MEMORY);

		res = m_socketStream.readStream(length, pBuf);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, delete[] pBuf, "readStream : %d", res);

		pBuf[length] = 0;

		*pI = * reinterpret_cast <int* > (pBuf);

		delete[] pBuf;

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int read(std::string* pStr)
	{
		int length = 0;
		int res = m_socketStream.readStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "readStream : %d", res);

		char* pBuf = new (std::nothrow) char[length + 1];
		TryReturn(pBuf != NULL, PRIV_MGR_ERROR_OUT_OF_MEMORY, , "new : %d", PRIV_MGR_ERROR_OUT_OF_MEMORY);

		m_socketStream.readStream(length, pBuf);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, delete[] pBuf, "readStream : %d", res);

		pBuf[length] = 0;

		*pStr = std::string(pBuf);
		delete[] pBuf;

		return PRIV_MGR_ERROR_SUCCESS;
	}
	
	int read(std::string& str)
	{
		return read(&str);
	}

	template < typename T >
	int  read (std::list<T>& list)
	{
		int length;
		int res = read(length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);

		for (int i = 0; i < length; ++i) 
		{
			T obj;
			res = read (obj);
			TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);
			list.push_back(obj);
		}

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template < typename T >
	int read (std::list<T>* pList)
	{
		return read(*pList);
	}

	template < typename K, typename P >
	void read (std::pair<K, P>& pair)
	{
		int res = read( pair.first);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);
		res = read( pair.second);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "read : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template < typename K, typename P >
	int read (std::pair<K, P>* pPair)
	{
		return read( *pPair);
	}

	template<typename T, typename ...Args>
	int write(const T& in, const Args&... args)
	{
		int res = write(in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		res = write(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int write(const std::string& in)
	{
		int length = in.size();
		int res = m_socketStream.writeStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);
		res = m_socketStream.writeStream(length, in.c_str());
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}
	
	int write(const unsigned int& in)
	{
		int length = sizeof(in);
		int res = m_socketStream.writeStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);
		res = m_socketStream.writeStream(length, &in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int write(const int& in)
	{
		int length = sizeof(in);
		int res = m_socketStream.writeStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);
		res = m_socketStream.writeStream(length, &in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int write(const bool& in)
	{
		int length = sizeof(in);
		int res = m_socketStream.writeStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);
		res = m_socketStream.writeStream(length, &in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	int write(const char*& in)
	{
		int length = strlen(in);
		int res = m_socketStream.writeStream(sizeof(length), &length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);
		res = m_socketStream.writeStream(length, in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "writeStream : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T, typename ...Args>
	int write(const T* in, const Args&... args)
	{
		int res = write(in);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		res = write(args...);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename K, typename T>
	int write(const std::pair<K, T> p)
	{
		int res = write(p.first);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		res = write(p.second);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename K, typename T>
	int write(const std::pair<K, T&> p)
	{
		int res = write(p.first);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		res = write(p.second);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename K, typename T>
	int write(const std::pair<K, T&>* pPair)
	{
		return write(*pPair);
	}

	template<typename T>
	int write(const std::list <T> list)
	{
		int length = list.size();
		int res = write(length);
		TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		for (typename std::list <T>::const_iterator iter = list.begin(); iter != list.end(); iter++) {
			res = write(*iter);
			TryReturn(res == PRIV_MGR_ERROR_SUCCESS, res, , "write : %d", res);
		}

		return PRIV_MGR_ERROR_SUCCESS;
	}

	template<typename T>
	int write(const std::list <T>* pList)
	{
		return write(*pList);
	}


private:
	SocketStream m_socketStream;
};

#endif // _SOCKET_CONNECTION_H_
