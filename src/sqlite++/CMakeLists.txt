PROJECT(sqlite++)

MESSAGE( STATUS "" )
MESSAGE( STATUS "<SQLite++>" )
FILE( GLOB decl y/sqlite++/*.hpp)
FILE( GLOB impl y/sqlite++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/sqlite++ )
SOURCE_GROUP( sqlite++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main)
INCLUDE_DIRECTORIES(../ext)

ADD_LIBRARY( y-sqlite++ STATIC ${srcs} )
INSTALL( TARGETS y-sqlite++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
MESSAGE( STATUS "<SQLite++/>" )
MESSAGE( STATUS "" )


