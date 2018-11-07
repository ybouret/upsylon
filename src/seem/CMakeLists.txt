PROJECT(seem CXX)
INCLUDE(../../scripts/dynamo.cmake)


Y_DYNAMO(y/seem/seem.g)

FILE( GLOB decl y/seem/*.hpp)
FILE( GLOB impl y/seem/*.cpp)
SET( srcs ${decl} ${impl} y/seem/seem.g y/seem/seem.def y/seem/seem.inc)
INSTALL( FILES ${decl} DESTINATION include/y/json )
SOURCE_GROUP( seem FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-seem STATIC ${srcs}  )
INSTALL( TARGETS y-seem ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
