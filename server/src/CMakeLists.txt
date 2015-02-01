CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#PROJECT(privacy-manager-server)

SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")
SET(VERSION 0.0)

SET(VERSION_MAJOR 0)
SET(VERSION "${VERSION_MAJOR}.0.1")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED dlog pkgmgr-info)

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

SET(src_dir "./")
SET(include_dir "./../inc/")
SET(common_src_dir "./../../common/src/")
SET(common_include_dir "./../../common/inc/")

## Additional flag
ADD_DEFINITIONS("-fvisibility=hidden")
ADD_DEFINITIONS("-Wall -Werror")
ADD_DEFINITIONS("-DDLOG_ERROR_ENABLED")

###################################################################################################
## for libprivacy-manager-server.so (library)
INCLUDE_DIRECTORIES(${pkgs_INCLUDE_DIRS})
SET(PRIVACY_MANAGER_SERVER_SOURCES 
	${src_dir}/main.cpp
	${src_dir}/SocketService.cpp
	${src_dir}/PrivacyManagerDaemon.cpp
	${common_src_dir}/SocketConnection.cpp
	${common_src_dir}/SocketStream.cpp
	)
SET(PRIVACY_MANAGER_SERVER_HEADERS
	${include_dir}/SocketService.h
	${include_dir}/PrivacyManagerDaemon.h
	${common_include_dir}/SocketConnection.h
	${common_include_dir}/SocketConnection.h
)
SET(PRIVACY_MANAGER_SERVER_LDFLAGS " -module -avoid-version ")
SET(PRIVACY_MANAGER_SERVER_CFLAGS  " ${CFLAGS} -fPIC -I${include_dir}" -I${common_include_dir})
#SET(PRIVACY_MANAGER_SERVER_LIBADD " ")

ADD_EXECUTABLE(privacy-manager-server ${PRIVACY_MANAGER_SERVER_SOURCES})
TARGET_LINK_LIBRARIES(privacy-manager-server ${pkgs_LDFLAGS} ${pkgs_LIBRARIES})
SET_TARGET_PROPERTIES(privacy-manager-server PROPERTIES COMPILE_FLAGS "${PRIVACY_MANAGER_SERVER_CFLAGS}")
SET_TARGET_PROPERTIES(privacy-manager-server PROPERTIES SOVERSION ${VERSION_MAJOR})
SET_TARGET_PROPERTIES(privacy-manager-server PROPERTIES VERSION ${VERSION})
###################################################################################################

CONFIGURE_FILE(../../privacy-manager-server.pc.in privacy-manager-server.pc @ONLY)

INSTALL(TARGETS privacy-manager-server DESTINATION ../lib COMPONENT RuntimeLibraries)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/privacy-manager-server.pc DESTINATION ../lib/pkgconfig)
INSTALL(FILES ${PRIVACY_MANAGER_SERVER_HEADERS} DESTINATION ../include)
