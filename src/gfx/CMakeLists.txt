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

FILE(GLOB async_decl y/gfx/async/*.hpp)
FILE(GLOB async_code y/gfx/async/*.cpp)
SET(async_srcs ${async_decl} ${async_code})
SOURCE_GROUP( gfx/async FILES ${async_srcs} )
INSTALL( FILES ${async_decl} DESTINATION include/y/gfx/async )

FILE(GLOB async_ops_decl y/gfx/async/ops/*.hpp)
FILE(GLOB async_ops_code y/gfx/async/ops/*.cpp)
SET(async_ops_srcs ${async_ops_decl} ${async_ops_code})
SOURCE_GROUP( gfx/async/ops FILES ${async_ops_srcs} )
INSTALL( FILES ${async_ops_decl} DESTINATION include/y/gfx/async/ops )



FILE(GLOB bitmap_decl y/gfx/bitmap/*.hpp)
FILE(GLOB bitmap_code y/gfx/bitmap/*.cpp)
SET(bitmap_srcs ${bitmap_decl} ${bitmap_code})
SOURCE_GROUP( gfx/bitmap FILES ${bitmap_srcs} )
INSTALL( FILES ${bitmap_decl} DESTINATION include/y/gfx/bitmap )

FILE(GLOB color_decl y/gfx/color/*.hpp)
FILE(GLOB color_code y/gfx/color/*.cpp)
SET(color_srcs ${color_decl} ${color_code})
SOURCE_GROUP( gfx/color FILES ${color_srcs} )
INSTALL( FILES ${color_decl} DESTINATION include/y/gfx/color )


ADD_LIBRARY(y-gfx STATIC 
${main_srcs}
${area_srcs}
${async_srcs} ${async_ops_srcs}
${bitmap_srcs}
${color_srcs}
)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )
 

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-gfx" PARENT_SCOPE)

MESSAGE( STATUS "<GFX/>" )
MESSAGE(STATUS "")
