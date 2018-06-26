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

FILE(GLOB type_decl y/type/*.hpp)
FILE(GLOB type_code y/type/*.cpp)
SET(type_srcs ${type_decl} ${type_code})
SOURCE_GROUP( type FILES ${type_srcs} )
INSTALL( FILES ${type_decl} DESTINATION include/y/type )

FILE(GLOB concurrent_decl y/concurrent/*.hpp)
FILE(GLOB concurrent_code y/concurrent/*.cpp)
SET(concurrent_srcs ${concurrent_decl} ${concurrent_code})
SOURCE_GROUP( concurrent FILES ${concurrent_srcs} )
INSTALL( FILES ${concurrent_decl} DESTINATION include/y/concurrent )

ADD_LIBRARY(y STATIC 
${main_srcs} 
${os_srcs}
${utest_srcs} 
${type_srcs}
${concurrent_srcs}
)
 
INSTALL( TARGETS y ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

