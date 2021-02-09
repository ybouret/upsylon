PROJECT(y-gfx CXX)

MESSAGE( STATUS "<GFX>" )

INCLUDE_DIRECTORIES(. ../main ../spade
../ext
../ext/y/tiff/libtiff
${CMAKE_CURRENT_BINARY_DIR}/../ext/y/tiff/libtiff)

FILE(GLOB main_decl y/gfx/*.hpp)
FILE(GLOB main_code y/gfx/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( gfx FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/gfx )

FILE(GLOB area_decl y/gfx/area/*.hpp)
FILE(GLOB area_code y/gfx/area/*.cpp)
SET(area_srcs ${area_decl} ${area_code})
SOURCE_GROUP( gfx/area FILES ${area_srcs} )
INSTALL( FILES ${area_decl} DESTINATION include/y/gfx/area )

ADD_LIBRARY(y-gfx STATIC 
${main_srcs}
${area_srcs}
)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )
 

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-gfx" PARENT_SCOPE)

MESSAGE( STATUS "<GFX/>" )
MESSAGE(STATUS "")
