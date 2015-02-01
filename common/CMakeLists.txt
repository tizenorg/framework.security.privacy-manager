CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(common_src_dir "${CMAKE_SOURCE_DIR}/common/src/")
SET(common_include_dir "${CMAKE_SOURCE_DIR}/common/inc/")

SET(PRIVACY_MANAGER_COMMON_SOURCES 
	${common_src_dir}/SocketConnection.cpp
	${common_src_dir}/SocketStream.cpp
	)

SET(PRIVACY_MANAGER_COMMON_HEADERS
	${common_include_dir}/SocketConnection.h
	${common_include_dir}/SocketConnection.h
)