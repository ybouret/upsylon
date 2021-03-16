PROJECT(y-GFX CXX)

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


FILE(GLOB image_decl y/gfx/image/*.hpp)
FILE(GLOB image_code y/gfx/image/*.cpp)
SET(image_srcs ${image_decl} ${image_code})
SOURCE_GROUP( gfx/image FILES ${image_srcs} )
INSTALL( FILES ${image_decl} DESTINATION include/y/gfx/image )

FILE(GLOB image_format_decl y/gfx/image/format/*.hpp)
FILE(GLOB image_format_code y/gfx/image/format/*.cpp)
SET(image_format_srcs ${image_format_decl} ${image_format_code})
SOURCE_GROUP( gfx/image/format FILES ${image_format_srcs} )
INSTALL( FILES ${image_format_decl} DESTINATION include/y/gfx/image/format )


FILE(GLOB ops_decl y/gfx/ops/*.hpp)
FILE(GLOB ops_code y/gfx/ops/*.cpp)
SET(ops_srcs ${ops_decl} ${ops_code})
SOURCE_GROUP( gfx/ops FILES ${ops_srcs} )
INSTALL( FILES ${ops_decl} DESTINATION include/y/gfx/ops )


FILE(GLOB filters_decl y/gfx/filters/*.hpp)
FILE(GLOB filters_code y/gfx/filters/*.cpp)
SET(filters_srcs ${filters_decl} ${filters_code})
SOURCE_GROUP( gfx/filters FILES ${filters_srcs} )
INSTALL( FILES ${filters_decl} DESTINATION include/y/gfx/filters )

ADD_LIBRARY(y-gfx STATIC 
${main_srcs}
${area_srcs}
${image_srcs} ${image_format_srcs}
${ops_srcs}
${filters_srcs}
)

INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-gfx" PARENT_SCOPE)

MESSAGE( STATUS "<GFX/>" )
MESSAGE(STATUS "")
