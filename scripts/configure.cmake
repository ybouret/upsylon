########################################################################
##
##
## This scripts configure a GENERATOR with COMPILERS
##
##
########################################################################
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)



########################################################################
##
## Dectecting BUILD_TYPE
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
## Dectecting GENERATOR
##
########################################################################
MESSAGE( STATUS "Detecting GENERATOR" )
SET(KNOWN_GENERATOR OFF) #check if we know it
SET(MULTI_GENERATOR OFF) #generator handles mutliple build: Xcode, Visual Studio...
SET(TRACE_COMPILERS OFF) #if trace of compiler version may be kept

#default
IF( "" STREQUAL "${GENERATOR}" )
	SET(GENERATOR "Unix Makefiles")
ENDIF()

IF(NOT KNOWN_GENERATOR)
	MESSAGE( FATAL_ERROR "Unknown GENERATOR=${GENERATOR}" )
ENDIF()
