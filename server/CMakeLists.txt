CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(privacy-manager)

SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED dlog sqlite3 dbus-1 dbus-glib-1 db-util pkgmgr-info capi-system-info security-server vconf gio-2.0 libsystemd-daemon)

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

SET(server_src_dir "${CMAKE_SOURCE_DIR}/server/src")
SET(server_include_dir "${CMAKE_SOURCE_DIR}/server/inc/")
SET(common_src_dir "${CMAKE_SOURCE_DIR}/common/src/")
SET(common_include_dir "${CMAKE_SOURCE_DIR}/common/inc/")
SET(dbus_include_dir "/usr/include/dbus-1.0")

## Additional flag
ADD_DEFINITIONS("-fvisibility=hidden")
ADD_DEFINITIONS("-Wall -Werror")
ADD_DEFINITIONS("-DDLOG_ERROR_ENABLED")
OPTION (FILTER_PRELOADED_PKG "FILTER PRELOADEDPKG BY LIST" ON)
IF(FILTER_PRELOADED_PKG)
	MESSAGE("FILTER PKGs BY PRELOADED PKG")
	ADD_DEFINITIONS("-D__FILTER_PRELOADED_PKG")
ENDIF(FILTER_PRELOADED_PKG)

OPTION (LOCATION_FILTER_LISTED_PKG "LOCATION FILTER PKG BY LIST" ON)
IF(LOCATION_FILTER_LISTED_PKG)
	MESSAGE("LOCATION FILTER PKGs BY FILTERING LIST")
	ADD_DEFINITIONS("-D__LOCATION_FILTER_LISTED_PKG")
ENDIF(LOCATION_FILTER_LISTED_PKG)

###################################################################################################
## for privacy-manager-server (executable)
INCLUDE_DIRECTORIES(
	${pkgs_INCLUDE_DIRS}
	${server_include_dir}
	${common_include_dir} 
	${dbus_include_dir}
	/usr/include/security-server/
	)

SET(PRIVACY_MANAGER_SERVER_SOURCES 
	${common_src_dir}/SocketConnection.cpp
	${common_src_dir}/SocketStream.cpp
	${common_src_dir}/PrivacyDb.cpp
	${common_src_dir}/PrivacyIdInfo.cpp
   ${server_src_dir}/main.cpp
	${server_src_dir}/SocketService.cpp
	${server_src_dir}/PrivacyManagerDaemon.cpp
	${server_src_dir}/service/PrivacyInfoService.cpp
	${server_src_dir}/PrivacyManagerServer.cpp
	${server_src_dir}/NotificationServer.cpp
	#${server_src_dir}/privacy_manager_daemon.cpp
	)
SET(PRIVACY_MANAGER_SERVER_HEADERS
	#${server_include_dir}/privacy_manager_daemon.h
    #	${server_include_dir}/SocketService.h
    #   ${server_include_dir}/PrivacyManagerDaemon.h
    #	${common_include_dir}/SocketConnection.h
    #	${common_include_dir}/SocketConnection.h
)
SET(PRIVACY_MANAGER_SERVER_LDFLAGS " -module -avoid-version")
SET(PRIVACY_MANAGER_SERVER_CFLAGS  " ${CFLAGS} -fPIE ")
#SET(PRIVACY_MANAGER_SERVER_LIBADD " ")

ADD_DEFINITIONS("-DLOG_TAG=\"PRIVACY-MANAGER-SERVER\"")
ADD_EXECUTABLE(privacy-manager-server ${PRIVACY_MANAGER_SERVER_SOURCES})
TARGET_LINK_LIBRARIES(privacy-manager-server ${pkgs_LDFLAGS} ${pkgs_LIBRARIES} "-ldl -pie")
SET_TARGET_PROPERTIES(privacy-manager-server PROPERTIES COMPILE_FLAGS "${PRIVACY_MANAGER_SERVER_CFLAGS}")
###################################################################################################

SET(PC_NAME privacy-manager-server)
SET(PC_DESCRIPTION "Privacy Manager Server API")
SET(PC_LDFLAGS -lprivacy-manager-server)
SET(PC_CFLAGS -I\${includedir}/privacy_manager)

CONFIGURE_FILE(../privacy-manager-server.pc.in privacy-manager-server.pc @ONLY)

INSTALL(TARGETS privacy-manager-server DESTINATION ../bin COMPONENT RuntimeLibraries)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/privacy-manager-server.pc DESTINATION ../lib/pkgconfig)
INSTALL(FILES ${PRIVACY_MANAGER_SERVER_HEADERS} DESTINATION ../include/privacy_manager/server)
