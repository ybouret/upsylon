PROJECT(y-m C CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/math/*.hpp)
FILE(GLOB main_code y/math/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
INSTALL( FILES ${main_decl} DESTINATION include/y/math )

FILE(GLOB kernel_decl y/math/kernel/*.h??)
FILE(GLOB kernel_code y/math/kernel/*.cpp)
SET(kernel_srcs ${kernel_decl} ${kernel_code})
SOURCE_GROUP( kernel FILES ${kernel_srcs} )
INSTALL( FILES ${kernel_decl} DESTINATION include/y/math/kernel )

FILE(GLOB io_decl y/math/io/*.hpp)
FILE(GLOB io_code y/math/io/*.cpp)
SET(io_srcs ${io_decl} ${io_code})
SOURCE_GROUP( io FILES ${io_srcs} )
INSTALL( FILES ${io_decl} DESTINATION include/y/math/io )

FILE(GLOB fcn_decl y/math/fcn/*.hpp)
FILE(GLOB fcn_code y/math/fcn/*.cpp)
SET(fcn_srcs ${fcn_decl} ${fcn_code})
SOURCE_GROUP( fcn FILES ${fcn_srcs} )
INSTALL( FILES ${fcn_decl} DESTINATION include/y/math/fcn )


IF(TRUE)
ADD_LIBRARY(y-m STATIC
${main_srcs}
${kernel_srcs}
${io_srcs}
${fcn_srcs}
)
INSTALL( TARGETS y-m ARCHIVE DESTINATION lib )
ENDIF()

ADD_SUBDIRECTORY(tests)
