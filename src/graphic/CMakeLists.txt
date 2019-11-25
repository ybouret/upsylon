PROJECT(y-graphic CXX)

MESSAGE( STATUS "<Graphic>" )

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/graphic/*.hpp)
FILE(GLOB main_code y/graphic/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( graphic FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/graphic )

FILE(GLOB color_decl y/graphic/color/*.hpp)
FILE(GLOB color_code y/graphic/color/*.cpp)
SET(color_srcs ${color_decl} ${color_code})
SOURCE_GROUP( graphic/color FILES ${color_srcs} )
INSTALL( FILES ${color_decl} DESTINATION include/y/graphic/color )

FILE(GLOB parallel_decl y/graphic/parallel/*.hpp)
FILE(GLOB parallel_code y/graphic/parallel/*.cpp)
SET(parallel_srcs ${parallel_decl} ${parallel_code})
SOURCE_GROUP( graphic/parallel FILES ${parallel_srcs} )
INSTALL( FILES ${parallel_decl} DESTINATION include/y/graphic/parallel )

ADD_LIBRARY(y-graphic STATIC 
${main_srcs} 
${color_srcs}
${parallel_srcs}
)
INSTALL( TARGETS y-graphic ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-graphic" PARENT_SCOPE)

MESSAGE( STATUS "<Graphic/>" )
MESSAGE(STATUS "")
