PROJECT(y-chemical CXX)
MESSAGE( STATUS "<Chemical>" )

INCLUDE_DIRECTORIES(. ../main ../mkl ../lua++ ../ext ../jive)

FILE(GLOB main_decl y/chemical/*.hpp)
FILE(GLOB main_code y/chemical/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( chemical FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/chemical )

FILE(GLOB eqs_decl y/chemical/eqs/*.hpp)
FILE(GLOB eqs_code y/chemical/eqs/*.cpp)
SET(eqs_srcs ${eqs_decl} ${eqs_code})
SOURCE_GROUP( chemical/eqs FILES ${eqs_srcs} )
INSTALL( FILES ${eqs_decl} DESTINATION include/y/chemical/eqs )

ADD_LIBRARY(y-chemical STATIC 
${main_srcs}
${eqs_srcs}
)
INSTALL( TARGETS y-chemical ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-chemical" PARENT_SCOPE)

MESSAGE( STATUS "<Chemical/>" )
MESSAGE(STATUS "")
