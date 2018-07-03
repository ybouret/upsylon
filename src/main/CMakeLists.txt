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

FILE(GLOB code_decl y/code/*.hpp)
FILE(GLOB code_code y/code/*.cpp)
SET(code_srcs ${code_decl} ${code_code})
SOURCE_GROUP( code FILES ${code_srcs} )
INSTALL( FILES ${code_decl} DESTINATION include/y/code )

FILE(GLOB core_decl   y/core/*.hpp)
FILE(GLOB core_code y/core/*.cpp)
SET(core_srcs ${core_decl} ${core_code})
SOURCE_GROUP( core FILES ${core_srcs} )
INSTALL( FILES ${core_decl} DESTINATION include/y/core )

FILE(GLOB randomized_decl y/randomized/*.hpp)
FILE(GLOB randomized_code y/randomized/*.cpp)
SET(randomized_srcs ${randomized_decl} ${randomized_code})
SOURCE_GROUP( randomized FILES ${randomized_srcs} )
INSTALL( FILES ${randomized_decl} DESTINATION include/y/randomized )

FILE(GLOB memory_decl   y/memory/*.hpp)
FILE(GLOB memory_code y/memory/*.cpp)
SET(memory_srcs ${memory_decl} ${memory_code})
SOURCE_GROUP( memory FILES ${memory_srcs} )
INSTALL( FILES ${memory_decl} DESTINATION include/y/memory )

FILE(GLOB string_decl   y/string/*.hpp)
FILE(GLOB string_code y/string/*.cpp)
SET(string_srcs ${string_decl} ${string_code})
SOURCE_GROUP( string FILES ${string_srcs} )
INSTALL( FILES ${string_decl} DESTINATION include/y/string )

FILE(GLOB hashing_decl   y/hashing/*.hpp)
FILE(GLOB hashing_code y/hashing/*.cpp)
SET(hashing_srcs ${hashing_decl} ${hashing_code})
SOURCE_GROUP( hashing FILES ${hashing_srcs} )
INSTALL( FILES ${hashing_decl} DESTINATION include/y/hashing )

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
${core_srcs}
${code_srcs}
${randomized_srcs}
${hashing_srcs}
${memory_srcs}
${string_srcs}
${concurrent_srcs}
)
 
INSTALL( TARGETS y ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

