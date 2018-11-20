PROJECT(y-fltk CXX)

INCLUDE(../../scripts/fltk.cmake)

INCLUDE_DIRECTORIES(. ../main)

IF(FLTK_FOUND)

FILE(GLOB main_decl y/fltk/*.hpp)
FILE(GLOB main_code y/fltk/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( fltk FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/fltk )


ADD_LIBRARY(y-fltk STATIC ${main_srcs})
ADD_SUBDIRECTORY(tests)

ENDIF()
