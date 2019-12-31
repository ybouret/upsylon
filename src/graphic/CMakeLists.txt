PROJECT(y-graphic CXX)

MESSAGE( STATUS "<Graphic>" )

INCLUDE_DIRECTORIES(. ../main
../ext
../ext/y/tiff/libtiff
${CMAKE_CURRENT_BINARY_DIR}/../ext/y/tiff/libtiff)

FILE(GLOB main_decl y/graphic/*.hpp)
FILE(GLOB main_code y/graphic/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( graphic FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/graphic )

FILE(GLOB color_decl y/graphic/color/*.h*)
FILE(GLOB color_code y/graphic/color/*.cpp)
SET(color_srcs ${color_decl} ${color_code})
SOURCE_GROUP( graphic/color FILES ${color_srcs} )
INSTALL( FILES ${color_decl} DESTINATION include/y/graphic/color )

FILE(GLOB color_ramp_decl y/graphic/color/ramp/*.hpp)
FILE(GLOB color_ramp_code y/graphic/color/ramp/*.cpp)
SET(color_ramp_srcs ${color_ramp_decl} ${color_ramp_code})
SOURCE_GROUP( graphic/color/ramp FILES ${color_ramp_srcs} )
INSTALL( FILES ${color_ramp_decl} DESTINATION include/y/graphic/color/ramp )

FILE(GLOB parallel_decl y/graphic/parallel/*.hpp)
FILE(GLOB parallel_code y/graphic/parallel/*.cpp)
SET(parallel_srcs ${parallel_decl} ${parallel_code})
SOURCE_GROUP( graphic/parallel FILES ${parallel_srcs} )
INSTALL( FILES ${parallel_decl} DESTINATION include/y/graphic/parallel )

FILE(GLOB ops_decl y/graphic/ops/*.hpp)
FILE(GLOB ops_code y/graphic/ops/*.cpp)
SET(ops_srcs ${ops_decl} ${ops_code})
SOURCE_GROUP( graphic/ops FILES ${ops_srcs} )
INSTALL( FILES ${ops_decl} DESTINATION include/y/graphic/ops )

FILE(GLOB ops_filter_decl y/graphic/ops/filter/*.hpp)
FILE(GLOB ops_filter_code y/graphic/ops/filter/*.cpp)
SET(ops_filter_srcs ${ops_filter_decl} ${ops_filter_code})
SOURCE_GROUP( graphic/ops/filter FILES ${ops_filter_srcs} )
INSTALL( FILES ${ops_filter_decl} DESTINATION include/y/graphic/ops/filter )

FILE(GLOB ops_edges_decl y/graphic/ops/edges/*.hpp)
FILE(GLOB ops_edges_code y/graphic/ops/edges/*.cpp)
SET(ops_edges_srcs ${ops_edges_decl} ${ops_edges_code})
SOURCE_GROUP( graphic/ops/edges FILES ${ops_edges_srcs} )
INSTALL( FILES ${ops_edges_decl} DESTINATION include/y/graphic/ops/edges )


FILE(GLOB image_decl y/graphic/image/*.hpp)
FILE(GLOB image_code y/graphic/image/*.cpp)
SET(image_srcs ${image_decl} ${image_code})
SOURCE_GROUP( graphic/image FILES ${image_srcs} )
INSTALL( FILES ${image_decl} DESTINATION include/y/graphic/image )

ADD_LIBRARY(y-graphic STATIC 
${main_srcs} 
${color_srcs} ${color_ramp_srcs}
${parallel_srcs}
${ops_srcs} ${ops_filter_srcs} ${ops_edges_srcs}
${image_srcs} 
)
INSTALL( TARGETS y-graphic ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(y_color_ramp y_color_ramp.cpp)
TARGET_LINK_Y(y_color_ramp)

ADD_EXECUTABLE(y_named_colors y_named_colors.cpp)
TARGET_LINK_Y(y_named_colors)

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-graphic" PARENT_SCOPE)

MESSAGE( STATUS "<Graphic/>" )
MESSAGE(STATUS "")
