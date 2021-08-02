PROJECT(y-alchemy CXX)
MESSAGE( STATUS "<Alchemy>" )

INCLUDE_DIRECTORIES(. ../main ../mkl ../lua++ ../ext ../jive)

FILE(GLOB main_decl y/alchemy/*.hpp)
FILE(GLOB main_code y/alchemy/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( alchemy FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/alchemy )


FILE(GLOB eqs_decl y/alchemy/eqs/*.hpp)
FILE(GLOB eqs_code y/alchemy/eqs/*.cpp)
SET(eqs_srcs ${eqs_decl} ${eqs_code})
SOURCE_GROUP( alchemy/eqs FILES ${eqs_srcs} )
INSTALL( FILES ${eqs_decl} DESTINATION include/y/alchemy/eqs ) 
 
ADD_LIBRARY(y-alchemy STATIC 
${main_srcs}
${eqs_srcs}
)
INSTALL( TARGETS y-alchemy ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-alchemy" PARENT_SCOPE)

MESSAGE( STATUS "<Alchemy/>" )
MESSAGE(STATUS "")
