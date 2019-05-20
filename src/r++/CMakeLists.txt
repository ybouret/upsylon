PROJECT(r++)

MESSAGE( STATUS "<r++>" )

SET(R_FOUND OFF)

FIND_PROGRAM(R R DOC "R Executable")
IF( ${R} STREQUAL R-NOTFOUND )
	MESSAGE( STATUS "Cannot find R" )
ELSE()
	MESSAGE( STATUS "Found R=<${R}>")
	SET(R_FOUND ON)
ENDIF()
	
IF(R_FOUND)

FILE( GLOB decl y/r++/*.hpp)
FILE( GLOB impl y/r++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/r++ )
SOURCE_GROUP( r++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main)

#ADD_LIBRARY( y-z++ STATIC ${srcs} )
#INSTALL( TARGETS y-z++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
ENDIF()

MESSAGE( STATUS "<r++/>" )
MESSAGE( STATUS "" )
