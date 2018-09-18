PROJECT(lua++)

FILE( GLOB decl y/lua++/*.hpp)
FILE( GLOB impl y/lua++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/lua++ )
SOURCE_GROUP( lua++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../ext ../main )

ADD_LIBRARY( y-lua++ STATIC ${srcs} )
INSTALL( TARGETS y-lua++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
