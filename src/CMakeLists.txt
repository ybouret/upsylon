CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(upsylon)

########################################################################
##
##
## <Setup>
##
##
########################################################################
INCLUDE(../etc/y.cmake)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../sdk)
MESSAGE( STATUS "SDK in ${CMAKE_INSTALL_PREFIX}")

FILE(GLOB SCRIPTS ../etc/*.cmake)
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
ADD_SUBDIRECTORY(mkl)
ADD_SUBDIRECTORY(jive)
ADD_SUBDIRECTORY(json)
ADD_SUBDIRECTORY(spade)
ADD_SUBDIRECTORY(mpi++)
#ADD_SUBDIRECTORY(alchemy)
ADD_SUBDIRECTORY(chemical)
ADD_SUBDIRECTORY(gfx)
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


#ADD_SUBDIRECTORY(python)

########################################################################
##
##
## Gathering Tests
##
##
########################################################################
STRING( STRIP ${UTESTS} UTESTS)
STRING( REPLACE " " ";" UTESTS ${UTESTS} )
MESSAGE(STATUS "regression: <${UTESTS}>")

ADD_CUSTOM_TARGET(regression
DEPENDS ${UTESTS}
COMMENT "Performed Regression Tests" )

