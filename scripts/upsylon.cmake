########################################################################
##
##
## global CMake parameters
##
##
########################################################################
MESSAGE("")
MESSAGE(STATUS "<upsylon>")
SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

########################################################################
##
##
## Find out Platform for specific flags
##
##
########################################################################
MESSAGE(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")

STRING(COMPARE EQUAL "Darwin"  ${CMAKE_SYSTEM_NAME} UPSYLON_DARWIN)
STRING(COMPARE EQUAL "FreeBSD" ${CMAKE_SYSTEM_NAME} UPSYLON_FREEBSD)
STRING(COMPARE EQUAL "Linux"   ${CMAKE_SYSTEM_NAME} UPSYLON_LINUX)
STRING(COMPARE EQUAL "SunOS"   ${CMAKE_SYSTEM_NAME} UPSYLON_SUNOS)
STRING(COMPARE EQUAL "OpenBSD" ${CMAKE_SYSTEM_NAME} UPSYLON_OPENBSD)
STRING(COMPARE EQUAL "Windows" ${CMAKE_SYSTEM_NAME} UPSYLON_WINDOWS)

########################################################################
##
##
## Guess 32/64 pits
##
##
########################################################################
SET(UPSYLON32 OFF)
SET(UPSYLON64 OFF)
INCLUDE(CheckTypeSize)
CHECK_TYPE_SIZE("void *" UPSYLON_POINTER_SIZE BUILTIN_TYPES_ONLY LANGUAGE C)
IF(NOT HAVE_UPSYLON_POINTER_SIZE)
	MESSAGE( FATAL_ERROR "unable to detect sizeof(void*)" )
ENDIF()

IF("${UPSYLON_POINTER_SIZE}" STREQUAL "4")
	SET(UPSYLON32 ON)
	MESSAGE(STATUS "Assuming: 32 bits")
ENDIF()

IF("${UPSYLON_POINTER_SIZE}" STREQUAL "8")
	SET(UPSYLON64 ON)
	MESSAGE(STATUS "Assuming: 64 bits")
ENDIF()

########################################################################
##
##
## Find compilers
##
##
########################################################################
GET_FILENAME_COMPONENT(UPSYLON_CC ${CMAKE_C_COMPILER} NAME )
MESSAGE(STATUS "C Compiler Name: ${UPSYLON_CC}")
SET(UPSYLON_KNOWN_COMPILER OFF)



########################################################################
##
## gnu
##
########################################################################
IF( "${UPSYLON_CC}" MATCHES "^gcc.*" )
	SET(UPSYLON_KNOWN_COMPILER ON)
	SET(UPSYLON_GNU            ON)
	SET(COMMON_C_FLAGS        "-Wall -Wextra -pipe" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")

	SET(COMMON_CXX_FLAGS        "-Wall -Wextra -pipe -fexceptions -Weffc++" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
ENDIF()

########################################################################
##
## clang
##
########################################################################
IF( "${UPSYLON_CC}" MATCHES "^clang.*" )
	SET(UPSYLON_KNOWN_COMPILER ON)
	SET(UPSYLON_CLANG          ON)
	
	SET(COMMON_C_FLAGS        "-Wall -Wextra -pipe" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")

	SET(COMMON_CXX_FLAGS        "-Wall -Wextra -pipe -fexceptions -Weffc++ ${PIC_FLAGS} ${SPECIAL_CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	
ENDIF()

########################################################################
##
## 'intel'
##
########################################################################
IF( "${UPSYLON_CC}" MATCHES "^icc.*" )
	SET(UPSYLON_INTEL          ON)
	SET(UPSYLON_KNOWN_COMPILER ON)

	SET(COMMON_C_FLAGS        "-Wall -pipe -wd981 ${PIC_FLAGS}" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")

	SET(COMMON_CXX_FLAGS        "-Wall -pipe -wd981 -fexceptions ${PIC_FLAGS}" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")
  
ENDIF()

########################################################################
##
## 'microsoft'
##
########################################################################
IF( "${UPSYLON_CC}" STREQUAL "cl.exe" )
	SET(UPSYLON_MICROSOFT      ON)
	SET(UPSYLON_KNOWN_COMPILER ON)
	
	SET(COMMON_C_FLAGS        "-nologo -D_CRT_SECURE_NO_WARNINGS -wd4996" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS}" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -Ox -DNDEBUG=1")

	SET(COMMON_CXX_FLAGS        "-nologo -EHsc -D_CRT_SECURE_NO_WARNINGS" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS}" )
  	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -Ox -DNDEBUG=1")

ENDIF()

IF(NOT UPSYLON_KNOWN_COMPILER)
	MESSAGE( FATAL_ERROR "Unhandled compiler [${UPSYLON_CC}]")
ENDIF()

