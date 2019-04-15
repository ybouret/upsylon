PROJECT(bz2++)

MESSAGE( STATUS "<BZ2++>" )
FILE( GLOB decl y/bz2++/*.hpp)
FILE( GLOB impl y/bz2++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/bz2++ )
SOURCE_GROUP( bz2++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../ext ../main )

ADD_LIBRARY( y-bz2++ STATIC ${srcs} )
INSTALL( TARGETS y-bz2++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
MESSAGE( STATUS "<BZ2++/>" )
