PROJECT(y C CXX)

INCLUDE_DIRECTORIES(.)

FILE(GLOB main_decl y/*.hpp)
FILE(GLOB main_code y/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
INSTALL( FILES ${main_decl} DESTINATION include/y )

FILE(GLOB os_decl y/os/*.hpp)
FILE(GLOB os_code y/os/*.cpp)
SET(os_srcs ${os_decl} ${os_code})
SOURCE_GROUP( os FILES ${os_srcs} )
INSTALL( FILES ${os_decl} DESTINATION include/y/os )

FILE(GLOB utest_decl y/utest/*.hpp)
FILE(GLOB utest_code y/utest/*.cpp)
SET(utest_srcs ${utest_decl} ${utest_code})
SOURCE_GROUP( utest FILES ${utest_srcs} )
INSTALL( FILES ${utest_decl} DESTINATION include/y/utest )

ADD_LIBRARY(y STATIC ${main_srcs} ${os_srcs} ${utest_srcs})
INSTALL( TARGETS y ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

