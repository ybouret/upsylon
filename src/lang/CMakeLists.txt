PROJECT(y-lang CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/lang/*.hpp)
FILE(GLOB main_code y/lang/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( lang FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/lang )


FILE(GLOB pattern_decl y/lang/pattern/*.hpp)
FILE(GLOB pattern_code y/lang/pattern/*.cpp)
SET(pattern_srcs ${pattern_decl} ${pattern_code})
SOURCE_GROUP( lang/pattern FILES ${pattern_srcs} )
INSTALL( FILES ${pattern_decl} DESTINATION include/y/lang/pattern )

ADD_LIBRARY(y-lang STATIC 
${main_srcs} 
${pattern_srcs}
)
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
