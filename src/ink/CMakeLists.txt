PROJECT(y-ink CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/ink/*.hpp)
FILE(GLOB main_code y/ink/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
INSTALL( FILES ${main_decl} DESTINATION include/y/ink )

FILE(GLOB color_decl y/ink/color/*.hpp)
FILE(GLOB color_code y/ink/color/*.cpp)
SET(color_srcs ${color_decl} ${color_code})
SOURCE_GROUP( color FILES ${color_srcs} )
INSTALL( FILES ${color_decl} DESTINATION include/y/ink/color )



ADD_LIBRARY(y-ink STATIC ${main_srcs} ${color_srcs})
INSTALL( TARGETS y-ink ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
