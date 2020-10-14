PROJECT(y-aqua CXX)
MESSAGE( STATUS "<Aqua>" )

INCLUDE_DIRECTORIES(. ../main ../mkl )

FILE(GLOB main_decl y/aqua/*.hpp)
FILE(GLOB main_code y/aqua/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( aqua FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/aqua )
 
FILE(GLOB engine_decl y/aqua/engine/*.hpp)
FILE(GLOB engine_code y/aqua/engine/*.cpp)
SET(engine_srcs ${engine_decl} ${engine_code})
SOURCE_GROUP( aqua/engine FILES ${engine_srcs} )
INSTALL( FILES ${engine_decl} DESTINATION include/y/aqua/engine )

ADD_LIBRARY(y-aqua STATIC 
${main_srcs}
${engine_srcs})
INSTALL( TARGETS y-aqua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-aqua" PARENT_SCOPE)

MESSAGE( STATUS "<Aqua/>" )
MESSAGE(STATUS "")
