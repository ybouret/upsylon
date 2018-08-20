PROJECT(z++)

FILE( GLOB decl y/z++/*.hpp)
FILE( GLOB impl y/z++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/z++ )
SOURCE_GROUP( z++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main)
## for local zlib
MESSAGE( STATUS "z++ binary dir=${CMAKE_CURRENT_BINARY_DIR}")
INCLUDE_DIRECTORIES(../ext ${CMAKE_CURRENT_BINARY_DIR}/../ext/y/zlib)

ADD_LIBRARY( y-z++ STATIC ${srcs} )
INSTALL( TARGETS y-z++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)



