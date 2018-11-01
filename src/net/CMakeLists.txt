PROJECT(y-net CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/net/*.hpp)
FILE(GLOB main_code y/net/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( net FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/net )

ADD_LIBRARY(y-net STATIC 
${main_srcs}
)
INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
