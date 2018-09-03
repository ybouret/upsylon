PROJECT(y-lang CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/lang/*.hpp)
FILE(GLOB main_code y/lang/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( lang FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/lang )

ADD_LIBRARY(y-lang STATIC 
${main_srcs} 
)
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
