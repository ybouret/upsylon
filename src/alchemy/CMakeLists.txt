PROJECT(y-alchemy CXX)
MESSAGE( STATUS "<Alchemy>" )

INCLUDE_DIRECTORIES(. ../main ../mkl ../lua++ ../ext ../jive)

FILE(GLOB main_decl y/alchemy/*.hpp)
FILE(GLOB main_code y/alchemy/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( alchemy FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/alchemy )


ADD_LIBRARY(y-alchemy STATIC 
${main_srcs}
)
INSTALL( TARGETS y-alchemy ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-alchemy" PARENT_SCOPE)

MESSAGE( STATUS "<Alchemy/>" )
MESSAGE(STATUS "")
