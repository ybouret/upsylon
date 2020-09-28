PROJECT(y-aqua CXX)
MESSAGE( STATUS "<Aqua>" )

INCLUDE_DIRECTORIES(. ../main ../mkl )

FILE(GLOB main_decl y/aqua/*.hpp)
FILE(GLOB main_code y/aqua/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( aqua FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/aqua )
 
FILE(GLOB solver_decl y/aqua/solver/*.hpp)
FILE(GLOB solver_code y/aqua/solver/*.cpp)
SET(solver_srcs ${solver_decl} ${solver_code})
SOURCE_GROUP( aqua/solver FILES ${solver_srcs} )
INSTALL( FILES ${solver_decl} DESTINATION include/y/aqua/solver )

ADD_LIBRARY(y-aqua STATIC 
${main_srcs}
${solver_srcs} )
INSTALL( TARGETS y-aqua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-aqua" PARENT_SCOPE)

MESSAGE( STATUS "<Aqua/>" )
MESSAGE(STATUS "")
