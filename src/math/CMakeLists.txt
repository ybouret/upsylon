PROJECT(y-m C CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/math/*.hpp)
FILE(GLOB main_code y/math/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
INSTALL( FILES ${main_decl} DESTINATION include/y/math )

FILE(GLOB kernel_decl y/math/kernel/*.hpp)
FILE(GLOB kernel_code y/math/kernel/*.cpp)
SET(kernel_srcs ${kernel_decl} ${kernel_code})
SOURCE_GROUP( kernel FILES ${kernel_srcs} )
INSTALL( FILES ${os_decl} DESTINATION include/y/math/kernel )

ADD_LIBRARY(y-m STATIC
${main_srcs}
${kernel_srcs}
)

INSTALL( TARGETS y-m ARCHIVE DESTINATION lib )
ADD_SUBDIRECTORY(tests)
