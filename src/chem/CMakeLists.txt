PROJECT(y-chem CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/chem/*.hpp)
FILE(GLOB main_code y/chem/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( chem FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/chem )

ADD_LIBRARY(y-chem STATIC 
${main_srcs} 
)
INSTALL( TARGETS y-chem ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
