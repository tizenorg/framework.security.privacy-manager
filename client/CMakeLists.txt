CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#PROJECT(privacy-manager-client)

SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

INCLUDE(FindPkgConfig)
pkg_check_modules(privacy-manager-client REQUIRED dlog sqlite3 dbus-1 dbus-glib-1 db-util pkgmgr-info)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS_PROFILING    " -g -pg")
SET(CMAKE_CXX_FLAGS_PROFILING  " -std=c++0x -g -pg")
SET(CMAKE_C_FLAGS_DEBUG        " -g")
SET(CMAKE_CXX_FLAGS_DEBUG      " -std=c++0x -g")
SET(CMAKE_C_FLAGS_RELEASE      " -g")
SET(CMAKE_CXX_FLAGS_RELEASE    " -std=c++0x -g")
SET(CMAKE_C_FLAGS_CCOV         " -g --coverage")
SET(CMAKE_CXX_FLAGS_CCOV       " -std=c++0x -g --coverage")

SET(client_src_dir "${CMAKE_SOURCE_DIR}/client/src")
SET(client_include_dir "${CMAKE_SOURCE_DIR}/client/inc/")
SET(common_src_dir "${CMAKE_SOURCE_DIR}/common/src/")
SET(common_include_dir "${CMAKE_SOURCE_DIR}/common/inc/")
#SET(lib_include_dir 
#	"/usr/include/dbus-1.0"
#	"/usr/include/aul"
#)

## Additional flag
ADD_DEFINITIONS("-fvisibility=hidden")
ADD_DEFINITIONS("-Wall -Werror")
ADD_DEFINITIONS("-DDLOG_ERROR_ENABLED")
OPTION (FILTER_LISTED_PKG "FILTER PKG BY LIST" ON)
IF(FILTER_LISTED_PKG)
    MESSAGE("FILTER PKGs BY FILTERING LIST")
    ADD_DEFINITIONS("-D__FILTER_LISTED_PKG")
ENDIF(FILTER_LISTED_PKG)
## SET PRIVACY_POPUP FLAG
OPTION (PRIVACY_POPUP "PRIVACY_POPUP" ON)
IF(PRIVACY_POPUP)
	MESSAGE("PRIVACY_POPUP IS ENABLED")
	ADD_DEFINITIONS("-D__PRIVACY_POPUP")
ENDIF(PRIVACY_POPUP)

###################################################################################################
## for libprivacy-manager-client (executable)
INCLUDE_DIRECTORIES(${pkgs_INCLUDE_DIRS} ${client_include_dir} ${common_include_dir} ${lib_include_dir})
SET(PRIVACY_MANAGER_CLIENT_SOURCES 
	${common_src_dir}/SocketConnection.cpp
	${common_src_dir}/SocketStream.cpp
	${common_src_dir}/PrivacyIdInfo.cpp
	${common_src_dir}/PrivacyDb.cpp
	${client_src_dir}/SocketClient.cpp
	${client_src_dir}/PrivacyManagerClient.cpp
	${client_src_dir}/PrivacyChecker.cpp
	${client_src_dir}/privacy_checker_client.cpp
	${client_src_dir}/privacy_info_client.cpp
	${client_src_dir}/privacy_manager_client.cpp
	)
SET(PRIVACY_MANAGER_CLIENT_HEADERS
    ${client_include_dir}/PrivacyManagerClient.h
	${client_include_dir}/PrivacyChecker.h
	${client_include_dir}/privacy_info_client.h
	${client_include_dir}/privacy_manager_client.h
	${client_include_dir}/privacy_checker_client.h
	${common_include_dir}/privacy_manager_client_types.h
)
SET(PRIVACY_MANAGER_CLIENT_LDFLAGS " -module -avoid-version ")
SET(PRIVACY_MANAGER_CLIENT_CFLAGS  " ${CFLAGS} -fPIC ")
#SET(PRIVACY_MANAGER_CLIENT_LIBADD " ")

ADD_DEFINITIONS("-DLOG_TAG=\"PRIVACY-MANAGER-CLIENT\"")
ADD_LIBRARY(privacy-manager-client SHARED ${PRIVACY_MANAGER_CLIENT_SOURCES})
TARGET_LINK_LIBRARIES(privacy-manager-client ${pkgs_LDFLAGS} ${pkgs_LIBRARIES})
SET_TARGET_PROPERTIES(privacy-manager-client PROPERTIES COMPILE_FLAGS "${PRIVACY_MANAGER_CLIENT_CFLAGS}")
SET_TARGET_PROPERTIES(privacy-manager-client PROPERTIES SOVERSION ${API_VERSION})
SET_TARGET_PROPERTIES(privacy-manager-client PROPERTIES VERSION ${VERSION})
###################################################################################################

SET(PC_NAME privacy-manager-client)
SET(PC_DESCRIPTION "Privacy Manager Client API")
SET(PC_LDFLAGS -lprivacy-manager-client)
SET(PC_CFLAGS -I\${includedir}/privacy_manager)

CONFIGURE_FILE(../privacy-manager-client.pc.in privacy-manager-client.pc @ONLY)

INSTALL(TARGETS privacy-manager-client DESTINATION ../lib COMPONENT RuntimeLibraries)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/privacy-manager-client.pc DESTINATION ../lib/pkgconfig)
INSTALL(FILES ${PRIVACY_MANAGER_CLIENT_HEADERS} DESTINATION ../include/privacy_manager)
