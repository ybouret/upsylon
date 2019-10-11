PROJECT(r++)

MESSAGE( STATUS "<r++>" )

INCLUDE(../../scripts/r.cmake)

	
IF(R_FOUND)

FILE( GLOB decl y/r++/*.hpp)
FILE( GLOB impl y/r++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/r++ )
SOURCE_GROUP( r++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main)

#ADD_LIBRARY( y-r++ STATIC ${srcs} )
#INSTALL( TARGETS y-r++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
#SET(UTESTS "${UTESTS} utest-r++" PARENT_SCOPE)
ENDIF()

MESSAGE( STATUS "<r++/>" )
MESSAGE( STATUS "" )
