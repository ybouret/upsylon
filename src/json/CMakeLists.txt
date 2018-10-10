PROJECT(json)
INCLUDE(../../scripts/dynamo.cmake)


Y_DYNAMO(y/json/json.g)

FILE( GLOB decl y/json/*.hpp)
FILE( GLOB impl y/json/*.cpp)
SET( srcs ${decl} ${impl} y/json/json.g y/json/json.def y/json/json.inc)
INSTALL( FILES ${decl} DESTINATION include/y/json )
SOURCE_GROUP( json FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-json STATIC ${srcs}  )
INSTALL( TARGETS y-json ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
