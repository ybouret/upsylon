########################################################################
##
##
## Global CMake parameters
##
##
########################################################################
MESSAGE("")
MESSAGE(STATUS "<upsylon>")
SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

# display a separator line
MACRO(Y_SEPLINE)
	MESSAGE( STATUS "************************************************" )
ENDMACRO(Y_SEPLINE)

########################################################################
##
##
## Find out Platform for specific flags
##
##
########################################################################
MESSAGE(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")

STRING(COMPARE EQUAL "Darwin"  ${CMAKE_SYSTEM_NAME} Y_DARWIN)
STRING(COMPARE EQUAL "FreeBSD" ${CMAKE_SYSTEM_NAME} Y_FREEBSD)
STRING(COMPARE EQUAL "Linux"   ${CMAKE_SYSTEM_NAME} Y_LINUX)
STRING(COMPARE EQUAL "SunOS"   ${CMAKE_SYSTEM_NAME} Y_SUNOS)
STRING(COMPARE EQUAL "OpenBSD" ${CMAKE_SYSTEM_NAME} Y_OPENBSD)
STRING(COMPARE EQUAL "Windows" ${CMAKE_SYSTEM_NAME} Y_WINDOWS)

########################################################################
##
##
## Guess 32/64 pits
##
##
########################################################################
SET(Y32 OFF)
SET(Y64 OFF)
INCLUDE(CheckTypeSize)
CHECK_TYPE_SIZE("void *" Y_POINTER_SIZE BUILTIN_TYPES_ONLY LANGUAGE C)
IF(NOT HAVE_Y_POINTER_SIZE)
	MESSAGE( FATAL_ERROR "unable to detect sizeof(void*)" )
ENDIF()

IF("${Y_POINTER_SIZE}" STREQUAL "4")
	SET(Y32 ON)
	MESSAGE(STATUS "Assuming: 32 bits")
ENDIF()

IF("${Y_POINTER_SIZE}" STREQUAL "8")
	SET(Y64 ON)
	MESSAGE(STATUS "Assuming: 64 bits")
ENDIF()


########################################################################
##
##
## <CompilerVersion>
##
##
########################################################################
SET(Y_COMPILER_VERSION "")
SET(Y_COMPILER_MAJOR   "")
SET(Y_COMPILER_MINOR   "")
SET(Y_COMPILER_PATCH   "")

# function will be call for gnu/clang/intel...
# TODO: change for MACRO ?
FUNCTION(Y_FIND_COMPILER_VERSION)
	# call cc --version
	EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} --version
					OUTPUT_VARIABLE Y_RAW_COMPILER_VERSION
					RESULT_VARIABLE Y_FIND_COMPILER_VERSION_SUCCESS
					ERROR_QUIET
					)
	IF( NOT ("0" STREQUAL "${Y_FIND_COMPILER_VERSION_SUCCESS}") )
		MESSAGE( FATAL_ERROR "Couldn't find compiler version")
	ENDIF()
	# get version pattern
	STRING( REGEX MATCH    "[0-9]+\\.[0-9]+\\.[0-9]+"         YC_VERSION "${Y_RAW_COMPILER_VERSION}" )
	STRING( REGEX REPLACE  "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" YC_MAJOR   "${YC_VERSION}" )
	STRING( REGEX REPLACE  "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" YC_MINOR   "${YC_VERSION}" )
	STRING( REGEX REPLACE  "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" YC_PATCH   "${YC_VERSION}" )
	MESSAGE( STATUS "[${Y_CC}] is <${YC_VERSION}>: major=${YC_MAJOR}, minor=${YC_MINOR}, patchlevel=${YC_PATCH}" )
	SET(Y_COMPILER_VERSION ${YC_VERSION} PARENT_SCOPE)
	SET(Y_COMPILER_MAJOR   ${YC_MAJOR}   PARENT_SCOPE)
	SET(Y_COMPILER_MINOR   ${YC_MINOR}   PARENT_SCOPE)
	SET(Y_COMPILER_PATCH   ${YC_PATCH}   PARENT_SCOPE)
ENDFUNCTION(Y_FIND_COMPILER_VERSION)


########################################################################
##
##
## <FindCompilers>
##
##
########################################################################
GET_FILENAME_COMPONENT(Y_CC ${CMAKE_C_COMPILER} NAME )
MESSAGE(STATUS "C Compiler Name: ${Y_CC}")
SET(Y_KNOWN_COMPILER OFF)



########################################################################
##
## gnu
##
########################################################################
IF( "${Y_CC}" MATCHES "^gcc.*" )
	SET(Y_KNOWN_COMPILER ON)
	SET(Y_GNU            ON)
	Y_FIND_COMPILER_VERSION()
	
	SET(SPECIFIC_CXX_FLAGS "")
	IF( "${Y_COMPILER_MAJOR}" GREATER 6 )
		MESSAGE(STATUS "GNU version 7+, deactivating -no-except-type")
		SET(SPECIFIC_CXX_FLAGS "-Wno-noexcept-type")
		#SET(SPECIFIC_CXX_FLAGS "${SPECIFIC_CXX_FLAGS} -std=c++11 -U__STDC_NO_ATOMICS__")
	ENDIF()
        
	SET(COMMON_C_FLAGS        "-Wall -Wextra -pipe" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	
	SET(COMMON_CXX_FLAGS        "-Wall -Wextra -pipe -fexceptions -Weffc++ ${SPECIFIC_CXX_FLAGS}" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
ENDIF()

########################################################################
##
## clang
##
########################################################################
IF( "${Y_CC}" MATCHES "^clang.*" )
	SET(Y_KNOWN_COMPILER ON)
	SET(Y_CLANG          ON)
	Y_FIND_COMPILER_VERSION()

	SET(COMMON_C_FLAGS        "-Wall -Wextra -pipe" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")

	SET(COMMON_CXX_FLAGS        "-Wall -Wextra -pipe -fexceptions -Weffc++")
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	
ENDIF()

########################################################################
##
## 'intel'
##
########################################################################
IF( "${Y_CC}" MATCHES "^icc.*" )
	SET(Y_INTEL          ON)
	SET(Y_KNOWN_COMPILER ON)
	Y_FIND_COMPILER_VERSION()

	SET(COMMON_C_FLAGS        "-Wall -pipe -wd981" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")

	SET(COMMON_CXX_FLAGS        "-Wall -pipe -wd981 -fexceptions" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")
  
ENDIF()

########################################################################
##
## 'microsoft'
##
########################################################################
IF( "${Y_CC}" STREQUAL "cl.exe" )
	SET(Y_MICROSOFT      ON)
	SET(Y_KNOWN_COMPILER ON)
	
	SET(COMMON_C_FLAGS        "-nologo -D_CRT_SECURE_NO_WARNINGS -wd4996" )
	SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS}" )
	SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -Ox -DNDEBUG=1")

	SET(COMMON_CXX_FLAGS        "-nologo -EHsc -D_CRT_SECURE_NO_WARNINGS" )
	SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS}" )
  	SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -Ox -DNDEBUG=1")

ENDIF()

IF(NOT Y_KNOWN_COMPILER)
	MESSAGE( FATAL_ERROR "Unhandled compiler [${Y_CC}]")
ENDIF()

########################################################################
##
##
## <FindCompilers/>
##
##
########################################################################


########################################################################
##
##
## <SpecificLinking>
##
##
########################################################################
MACRO(TARGET_LINK_Y tgt)

	SET(ylibs "")
	
	# populate with dependendcy
	FOREACH( extra ${ARGN} )
		LIST( APPEND ylibs "y-${extra}" )
		
		IF( ${extra} STREQUAL "z++" )
			LIST(APPEND ylibs "y-z")
		ENDIF()
		
		IF( ${extra} STREQUAL "bz2++" )
			LIST(APPEND ylibs "y-bz2")
		ENDIF()
		
		IF( ${extra} STREQUAL "png" )
			LIST(APPEND ylibs "y-z")
		ENDIF()
		
		IF( ${extra} STREQUAL "tiff" )
			LIST(APPEND ylibs "y-jpeg;y-jbig;y-png;y-z")
		ENDIF()
		
		IF( ${extra} STREQUAL "ink" )
			LIST(APPEND ylibs "y-tiff;y-jpeg;y-jbig;y-png;y-z")
		ENDIF()
			
		IF( ${extra} STREQUAL "lua++" )
			LIST(APPEND ylibs "y-lua")
		ENDIF()
		
		IF( ${extra} STREQUAL "chem" )
			LIST(APPEND ylibs "y-m;y-lua++;y-lua")
		ENDIF()
		
		IF( ${extra} STREQUAL "json" )
			LIST(APPEND ylibs "y-lang")
		ENDIF()
		
		IF( ${extra} STREQUAL "seem" )
			LIST(APPEND ylibs "y-lang")
		ENDIF()
		
		IF( ${extra} STREQUAL "net" )
			IF( Y_GNU AND WIN32)
			LIST(APPEND ylibs "ws2_32")
			ENDIF()
			IF( Y_SUNOS )
			LIST(APPEND ylibs "socket")
			ENDIF()
		ENDIF()
		
	ENDFOREACH()
	
	#finalize
	LIST(APPEND ylibs y)
	
	#specific libraries
	LIST(REMOVE_DUPLICATES ylibs)
	MESSAGE(STATUS "[${tgt}]\t<${ylibs}>")
	TARGET_LINK_LIBRARIES(${tgt} ${ylibs})
	
	IF(Y_FREEBSD OR Y_OPENBSD)
		TARGET_LINK_LIBRARIES(${tgt} pthread)
	ENDIF()

	IF(Y_LINUX)
		TARGET_LINK_LIBRARIES(${tgt} pthread rt)
	ENDIF()
	
	IF(NOT WIN32)
		IF(NOT Y_OPENBSD)
			TARGET_LINK_LIBRARIES(${tgt} dl)
		ENDIF()
	ENDIF()
	
	#specific flags
	IF( Y_GNU AND WIN32)
		#MESSAGE( STATUS "gnu+win32" )
		IF("${Y_COMPILER_VERSION}" VERSION_GREATER "4.5.0" )
			#MESSAGE( STATUS "static gcc/g++ linking for ${tgt}")
			TARGET_LINK_LIBRARIES( ${tgt} -static-libgcc -static-libstdc++ )
		ENDIF()
	ENDIF()
	
	
	IF("${CMAKE_GENERATOR}" STREQUAL "Xcode")
		TARGET_LINK_LIBRARIES(${tgt} -w )
	ENDIF()
	
ENDMACRO(TARGET_LINK_Y)

########################################################################
##
##
## <SpecificLinking/>
##
##
########################################################################

