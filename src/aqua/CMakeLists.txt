PROJECT(y-aqua CXX)
MESSAGE( STATUS "<Aqua>" )

INCLUDE_DIRECTORIES(. ../main ../mkl )

FILE(GLOB main_decl y/aqua/*.hpp)
FILE(GLOB main_code y/aqua/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( aqua FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/aqua )
 
FILE(GLOB eqs_decl y/aqua/eqs/*.hpp)
FILE(GLOB eqs_code y/aqua/eqs/*.cpp)
SET(eqs_srcs ${eqs_decl} ${eqs_code})
SOURCE_GROUP( aqua FILES ${eqs_srcs} )
INSTALL( FILES ${eqs_decl} DESTINATION include/y/aqua/eqs )

ADD_LIBRARY(y-aqua STATIC 
${main_srcs}
${eqs_srcs} )
INSTALL( TARGETS y-aqua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-aqua" PARENT_SCOPE)

MESSAGE( STATUS "<Aqua/>" )
MESSAGE(STATUS "")
