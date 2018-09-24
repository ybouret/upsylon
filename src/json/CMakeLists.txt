PROJECT(json)

FILE( GLOB decl y/json/*.hpp)
FILE( GLOB impl y/json/*.cpp)
SET( srcs ${decl} ${impl} y/json/json.g)
INSTALL( FILES ${decl} DESTINATION include/y/json )
SOURCE_GROUP( json FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-json STATIC ${srcs}  )
INSTALL( TARGETS y-json ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
