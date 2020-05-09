CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
PROJECT(upsylon)

########################################################################
##
##
## <Setup>
##
##
########################################################################
INCLUDE(../scripts/y.cmake)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../sdk)
MESSAGE( STATUS "SDK in ${CMAKE_INSTALL_PREFIX}")

FILE(GLOB SCRIPTS ../scripts/*.cmake)
INSTALL( FILES ${SCRIPTS} DESTINATION share/y )

########################################################################
##
##
## <Setup/>
##
##
########################################################################

########################################################################
##
##
## <Libraries>
##
##
########################################################################
SET(UTESTS "" )

MESSAGE(STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning Libraries>" )
Y_SEPLINE()
MESSAGE(STATUS "")
ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(math)
ADD_SUBDIRECTORY(jargon)
ADD_SUBDIRECTORY(json)
ADD_SUBDIRECTORY(spade)
ADD_SUBDIRECTORY(mpi)
ADD_SUBDIRECTORY(mpi++)
ADD_SUBDIRECTORY(chem)
ADD_SUBDIRECTORY(graphic)
ADD_SUBDIRECTORY(net)
ADD_SUBDIRECTORY(omp)
MESSAGE(STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning Libraries/>" )
Y_SEPLINE()
MESSAGE(STATUS "")
########################################################################
##
##
## <Libraries/>
##
##
########################################################################

########################################################################
##
##
## <Doxygen>
##
##
########################################################################
FIND_PROGRAM(DOXYGEN doxygen)
IF( "${DOXYGEN}" STREQUAL "DOXYGEN-NOTFOUND"  )
	MESSAGE( STATUS "No available Doxygen" )
ELSE()
	MESSAGE( STATUS "Will generate Doxygen" ) 
	ADD_CUSTOM_TARGET(docs
		COMMAND ${DOXYGEN}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
		COMMENT "Generating Documentation")
ENDIF()
########################################################################
##
##
## <Doxygen/>
##
##
########################################################################

########################################################################
##
##
## <External>
##
##
########################################################################
MESSAGE( STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning External Libraries>" )
Y_SEPLINE()
MESSAGE( STATUS "")

ADD_SUBDIRECTORY(ext)

MESSAGE( STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning External Libraries/>" )
Y_SEPLINE()
MESSAGE( STATUS "")


########################################################################
##
##
## <External/>
##
##
########################################################################



########################################################################
##
##
## <Interface>
##
##
########################################################################
MESSAGE(STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning C++ Wrapping Libraries>" )
Y_SEPLINE()
MESSAGE(STATUS "")

ADD_SUBDIRECTORY(z++)
ADD_SUBDIRECTORY(bz2++)
ADD_SUBDIRECTORY(lua++)
ADD_SUBDIRECTORY(sqlite++)
ADD_SUBDIRECTORY(fltk)
ADD_SUBDIRECTORY(r++)

MESSAGE(STATUS "")
Y_SEPLINE()
MESSAGE( STATUS "<Scanning C++ Wrapping Libraries/>" )
Y_SEPLINE()
MESSAGE(STATUS "")

########################################################################
##
##
## <Interface/>
##
##
########################################################################

ADD_CUSTOM_TARGET(core
DEPENDS y y-lang y-m y-chem y-net
COMMENT "Built Core Libraries"
)

########################################################################
##
##
## Gathering Tests
##
##
########################################################################
#SET(UTESTS  utest-main utest-m utest-chem utest-lang utest-net utest-ink)
STRING( STRIP ${UTESTS} UTESTS)
STRING( REPLACE " " ";" UTESTS ${UTESTS} )
MESSAGE(STATUS "regression: <${UTESTS}>")

ADD_CUSTOM_TARGET(regression
DEPENDS ${UTESTS}
COMMENT "Performed Regression Tests" )

