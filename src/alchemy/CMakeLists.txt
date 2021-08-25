PROJECT(y-alchemy CXX)
MESSAGE( STATUS "<Alchemy>" )

INCLUDE_DIRECTORIES(. ../main ../mkl ../lua++ ../ext ../jive)

FILE(GLOB main_decl y/alchemy/*.hpp)
FILE(GLOB main_code y/alchemy/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( alchemy FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/alchemy )

FILE(GLOB lib_decl y/alchemy/lib/*.hpp)
FILE(GLOB lib_code y/alchemy/lib/*.cpp)
SET(lib_srcs ${lib_decl} ${lib_code})
SOURCE_GROUP( alchemy/lib FILES ${lib_srcs} )
INSTALL( FILES ${lib_decl} DESTINATION include/y/alchemy/lib ) 

FILE(GLOB eqs_decl y/alchemy/eqs/*.hpp)
FILE(GLOB eqs_code y/alchemy/eqs/*.cpp)
SET(eqs_srcs ${eqs_decl} ${eqs_code})
SOURCE_GROUP( alchemy/eqs FILES ${eqs_srcs} )
INSTALL( FILES ${eqs_decl} DESTINATION include/y/alchemy/eqs ) 
 
 
FILE(GLOB reactor_decl y/alchemy/reactor/*.hpp)
FILE(GLOB reactor_code y/alchemy/reactor/*.cpp)
SET(reactor_srcs ${reactor_decl} ${reactor_code})
SOURCE_GROUP( alchemy/reactor FILES ${reactor_srcs} )
INSTALL( FILES ${reactor_decl} DESTINATION include/y/alchemy/reactor ) 
 
ADD_LIBRARY(y-alchemy STATIC 
${main_srcs}
${lib_srcs}
${eqs_srcs}
${reactor_srcs}
)
INSTALL( TARGETS y-alchemy ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-alchemy" PARENT_SCOPE)

MESSAGE( STATUS "<Alchemy/>" )
MESSAGE(STATUS "")
