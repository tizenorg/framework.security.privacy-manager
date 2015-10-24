CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(this_target privacy-manager-client)
SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

INCLUDE(FindPkgConfig)
pkg_check_modules(${this_target} REQUIRED dlog sqlite3 dbus-1 dbus-glib-1 db-util pkgmgr-info capi-system-info vconf aul)

FOREACH(flag ${${this_target}_CFLAGS})
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
#OPTION (FILTER_LISTED_PKG "FILTER PKG BY LIST" ON)
#IF(FILTER_LISTED_PKG)
#    MESSAGE("FILTER PKGs BY FILTERING LIST")
#   ADD_DEFINITIONS("-D__FILTER_LISTED_PKG")
#ENDIF(FILTER_LISTED_PKG)
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
## for lib${this_target} (executable)
INCLUDE_DIRECTORIES(${${this_target}_INCLUDE_DIRS} ${client_include_dir} ${common_include_dir} ${lib_include_dir})
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
ADD_LIBRARY(${this_target} SHARED ${PRIVACY_MANAGER_CLIENT_SOURCES})
TARGET_LINK_LIBRARIES(${this_target} ${${this_target}_LDFLAGS} ${${this_target}_LIBRARIES})
SET_TARGET_PROPERTIES(${this_target} PROPERTIES COMPILE_FLAGS "${PRIVACY_MANAGER_CLIENT_CFLAGS}")
SET_TARGET_PROPERTIES(${this_target} PROPERTIES SOVERSION ${API_VERSION})
SET_TARGET_PROPERTIES(${this_target} PROPERTIES VERSION ${VERSION})
###################################################################################################

SET(PC_NAME ${this_target})
SET(PC_DESCRIPTION "Privacy Manager Client API")
SET(PC_LDFLAGS "-l${this_target}")
SET(PC_CFLAGS -I\${includedir}/privacy_manager)

CONFIGURE_FILE(../${this_target}.pc.in ${this_target}.pc @ONLY)

INSTALL(TARGETS ${this_target} DESTINATION ../lib COMPONENT RuntimeLibraries)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${this_target}.pc DESTINATION ../lib/pkgconfig)
INSTALL(FILES ${PRIVACY_MANAGER_CLIENT_HEADERS} DESTINATION ../include/privacy_manager)
