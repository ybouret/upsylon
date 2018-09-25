PROJECT(y-chem CXX)

INCLUDE_DIRECTORIES(. ../main ../math ../lua++ ../ext)

FILE(GLOB main_decl y/chem/*.hpp)
FILE(GLOB main_code y/chem/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( chem FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/chem )

#FILE(GLOB eqs_decl y/chem/eqs/*.hpp)
FILE(GLOB eqs_code y/chem/eqs/*.cpp)
SOURCE_GROUP( eqs FILES ${eqs_code} )
#INSTALL( FILES ${main_decl} DESTINATION include/y/chem )

FILE(GLOB lua_decl y/chem/lua/*.hpp)
FILE(GLOB lua_code y/chem/lua/*.cpp)
SET(lua_srcs ${lua_decl} ${lua_code})
SOURCE_GROUP( lua FILES ${lua_srcs} )
INSTALL( FILES ${lua_decl} DESTINATION include/y/chem/lua )


ADD_LIBRARY(y-chem STATIC 
${main_srcs} 
${eqs_code}
${lua_srcs}
)
INSTALL( TARGETS y-chem ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
