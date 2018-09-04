PROJECT(y-chem CXX)

INCLUDE_DIRECTORIES(. ../main ../math)

FILE(GLOB main_decl y/chem/*.hpp)
FILE(GLOB main_code y/chem/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( chem FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/chem )

#FILE(GLOB eqs_decl y/chem/eqs/*.hpp)
FILE(GLOB eqs_code y/chem/eqs/*.cpp)
#SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( eqs FILES ${eqs_code} )
#INSTALL( FILES ${main_decl} DESTINATION include/y/chem )


ADD_LIBRARY(y-chem STATIC 
${main_srcs} 
${eqs_code}
)
INSTALL( TARGETS y-chem ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
