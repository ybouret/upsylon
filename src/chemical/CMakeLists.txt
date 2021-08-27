PROJECT(y-chemical CXX)
MESSAGE( STATUS "<Chemical>" )

INCLUDE_DIRECTORIES(. ../main ../mkl ../lua++ ../ext ../jive)

FILE(GLOB main_decl y/chemical/*.hpp)
FILE(GLOB main_code y/chemical/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( chemical FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/chemical )

ADD_LIBRARY(y-chemical STATIC 
${main_srcs}
)
INSTALL( TARGETS y-chemical ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-chemical" PARENT_SCOPE)

MESSAGE( STATUS "<Chemical/>" )
MESSAGE(STATUS "")
