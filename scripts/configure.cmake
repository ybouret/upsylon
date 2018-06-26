########################################################################
##
##
##
## This scripts configure a GENERATOR with COMPILERS
##
##
##
########################################################################
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)



########################################################################
##
##
## Dectecting BUILD_TYPE
##
##
########################################################################
MESSAGE( STATUS "Detecting BUILD_TYPE [Debug|Release]" )
SET(KNOW_BUILD_TYPE OFF)

#default
IF("" STREQUAL "${BUILD_TYPE}")
	SET(BUILD_TYPE "Debug")
ENDIF()

IF("Debug" STREQUAL "${BUILD_TYPE}")
	SET(KNOWN_BUILD_TYPE ON)
ENDIF()

IF("Release" STREQUAL "${BUILD_TYPE}")
	SET(KNOWN_BUILD_TYPE ON)
ENDIF()

IF(NOT KNOWN_BUILD_TYPE)
	MESSAGE( FATAL_ERROR "Unknown BUILD_TYPE=${BUILD_TYPE}" )
ELSE()
	MESSAGE( STATUS "          BUILD_TYPE=${BUILD_TYPE}" )
ENDIF()

########################################################################
##
##
## Dectecting GENERATOR
##
##
########################################################################
MESSAGE( STATUS "Detecting GENERATOR" )
SET(KNOWN_GENERATOR OFF) #check if we know it
SET(MULTI_GENERATOR OFF) #generator handles mutliple builds (Xcode, Visual Studio...): default is OFF
SET(TRACE_COMPILERS ON)  #if trace of compiler version may be kept (gcc,clang...)    : default is ON

#default
IF( "" STREQUAL "${GENERATOR}" )
	SET(GENERATOR "Unix Makefiles")
ENDIF()

########################################################################
##
## Unix Makefiles: single output with compilers version
##
########################################################################
IF( "${GENERATOR}" STREQUAL "Unix Makefiles" )
	SET(KNOWN_GENERATOR ON)
ENDIF()

########################################################################
##
## Ninja: single output with compilers version
##
########################################################################
IF( "${GENERATOR}" STREQUAL "Ninja" )
	SET(KNOWN_GENERATOR ON)
ENDIF()

########################################################################
##
## Visual Studio: mutliple outputs, single compilers
##
########################################################################
IF( "${GENERATOR}" MATCHES "Visual Studio.*" )
	SET(KNOWN_GENERATOR ON)
	SET(MULTI_GENERATOR ON)
	SET(COMPILERS "microsoft") #cl.exe in MSVS
	SET(TRACE_COMPILERS OFF)
ENDIF()

########################################################################
##
## Codeblocks: single output with compilers version
##
########################################################################
IF( "${GENERATOR}" MATCHES "CodeBlocks.*" )
	SET(KNOWN_GENERATOR ON)
ENDIF()

########################################################################
##
## Kate: single output with compilers version
##
########################################################################
IF( "${GENERATOR}" MATCHES "Kate - .*" )
	SET(KNOWN_GENERATOR ON)
ENDIF()

########################################################################
##
## Xcode: mutliple outputs, single compilers
##
########################################################################
IF( "${GENERATOR}" STREQUAL "Xcode" )
	SET(KNOWN_GENERATOR  ON)
	SET(MULTI_GENERATOR  ON)
	SET(COMPILERS   "clang") #AppleClang in Xcode
	SET(TRACE_COMPILERS OFF)
ENDIF()

########################################################################
##
## CodeLite: single output with compilers version
##
########################################################################
IF( "${GENERATOR}" MATCHES "CodeLite - .*" )
	SET(KNOWN_GENERATOR ON)
ENDIF()

IF(NOT KNOWN_GENERATOR)
	MESSAGE( FATAL_ERROR "Unknown GENERATOR=${GENERATOR}" )
ELSE()
	MESSAGE( STATUS "          GENERATOR=${GENERATOR}" )
ENDIF()


########################################################################
##
##
## Dectecting COMPILERS
##
##
########################################################################
MESSAGE( STATUS "Detecting COMPILERS" )
