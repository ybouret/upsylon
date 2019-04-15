PROJECT(y-ink CXX)
MESSAGE( STATUS "<Images Numerical Kernel>" )

INCLUDE_DIRECTORIES(. ../main
../ext
../ext/y/tiff/libtiff
${CMAKE_CURRENT_BINARY_DIR}/../ext/y/tiff/libtiff)

FILE(GLOB main_decl y/ink/*.hpp)
FILE(GLOB main_code y/ink/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( ink FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/ink )

FILE(GLOB color_decl y/ink/color/*.h??)
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

FILE(GLOB ops_filter_decl y/ink/ops/filter/*.hxx)
SET(ops_filter_srcs ${ops_filter_decl} )
SOURCE_GROUP( ops/filter FILES ${ops_filter_srcs} )
INSTALL( FILES ${ops_filter_decl} DESTINATION include/y/ink/ops/filter )


FILE(GLOB stencil_decl y/ink/stencil/*.hpp)
FILE(GLOB stencil_code y/ink/stencil/*.cpp)
SET(stencil_srcs ${stencil_decl} ${stencil_code})
SOURCE_GROUP( stencil FILES ${stencil_srcs} )
INSTALL( FILES ${stencil_decl} DESTINATION include/y/ink/stencil )

FILE(GLOB draw_decl y/ink/draw/*.hpp)
FILE(GLOB draw_code y/ink/draw/*.cpp)
SET(draw_srcs ${draw_decl} ${draw_code})
SOURCE_GROUP( draw FILES ${draw_srcs} )
INSTALL( FILES ${draw_decl} DESTINATION include/y/ink/draw )

ADD_LIBRARY(y-ink STATIC 
${main_srcs} 
${color_srcs} 
${image_srcs}
${ops_srcs}
${ops_filter_srcs}
${stencil_srcs}
${draw_srcs}
)
INSTALL( TARGETS y-ink ARCHIVE DESTINATION lib )

#ADD_EXECUTABLE(named-colors named-colors.cpp)
#TARGET_LINK_LIBRARIES(named-colors y)

ADD_SUBDIRECTORY(tests)
MESSAGE( STATUS "<Images Numerical Kernel/>" )
