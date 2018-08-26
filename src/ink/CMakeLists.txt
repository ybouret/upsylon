PROJECT(y-ink CXX)

INCLUDE_DIRECTORIES(. ../main
../ext
../ext/y/tiff/libtiff
${CMAKE_CURRENT_BINARY_DIR}/../ext/y/tiff/libtiff)

FILE(GLOB main_decl y/ink/*.hpp)
FILE(GLOB main_code y/ink/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( ink FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/ink )

FILE(GLOB color_decl y/ink/color/*.hpp)
FILE(GLOB color_code y/ink/color/*.cpp)
SET(color_srcs ${color_decl} ${color_code})
SOURCE_GROUP( color FILES ${color_srcs} )
INSTALL( FILES ${color_decl} DESTINATION include/y/ink/color )


FILE(GLOB image_decl y/ink/image/*.hpp)
FILE(GLOB image_code y/ink/image/*.cpp)
SET(image_srcs ${image_decl} ${image_code})
SOURCE_GROUP( image FILES ${image_srcs} )
INSTALL( FILES ${image_decl} DESTINATION include/y/ink/image )

FILE(GLOB ops_decl y/ink/ops/*.hpp)
FILE(GLOB ops_code y/ink/ops/*.cpp)
SET(ops_srcs ${ops_decl} ${ops_code})
SOURCE_GROUP( ops FILES ${ops_srcs} )
INSTALL( FILES ${ops_decl} DESTINATION include/y/ink/ops )

ADD_LIBRARY(y-ink STATIC 
${main_srcs} 
${color_srcs} 
${image_srcs}
${ops_srcs}
)
INSTALL( TARGETS y-ink ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
