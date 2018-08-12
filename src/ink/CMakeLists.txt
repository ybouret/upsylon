PROJECT(y-ink CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/ink/*.hpp)
FILE(GLOB main_code y/ink/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
INSTALL( FILES ${main_decl} DESTINATION include/y/ink )

#FILE(GLOB kernel_decl y/math/kernel/*.h??)
#FILE(GLOB kernel_code y/math/kernel/*.cpp)
#SET(kernel_srcs ${kernel_decl} ${kernel_code})
#SOURCE_GROUP( kernel FILES ${kernel_srcs} )
#NSTALL( FILES ${kernel_decl} DESTINATION include/y/math/kernel )



ADD_LIBRARY(y-ink STATIC ${main_srcs} )
INSTALL( TARGETS y-ink ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
