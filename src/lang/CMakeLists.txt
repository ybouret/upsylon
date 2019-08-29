PROJECT(y-lang CXX)

MESSAGE( STATUS "<Lang>")

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/lang/*.hpp)
FILE(GLOB main_code y/lang/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( lang FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/lang )


FILE(GLOB syntax_decl y/lang/syntax/*.hpp)
FILE(GLOB syntax_code y/lang/syntax/*.cpp)
SET(syntax_srcs ${syntax_decl} ${syntax_code})
SOURCE_GROUP( lang/syntax FILES ${syntax_srcs} )
INSTALL( FILES ${syntax_decl} DESTINATION include/y/lang/syntax )

FILE(GLOB syntax_grammar_srcs y/lang/syntax/grammar/*.cpp)
SOURCE_GROUP( lang/syntax/grammar FILES ${syntax_grammar_srcs} )

FILE(GLOB syntax_node_srcs y/lang/syntax/node/*.cpp)
SOURCE_GROUP( lang/syntax/node FILES ${syntax_node_srcs} )

FILE(GLOB dynamo_decl y/lang/dynamo/*.hpp)
FILE(GLOB dynamo_code y/lang/dynamo/*.cpp)
SET(dynamo_srcs ${dynamo_decl} ${dynamo_code})
SOURCE_GROUP( lang/dynamo FILES ${dynamo_srcs} )
INSTALL( FILES ${dynamo_decl} DESTINATION include/y/lang/dynamo )

ADD_LIBRARY(y-lang STATIC 
${main_srcs} 
${syntax_srcs}
${syntax_grammar_srcs}
${syntax_node_srcs}
${dynamo_srcs}
)
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-lang" PARENT_SCOPE)

MESSAGE( STATUS "<Lang/>")
MESSAGE(STATUS "")
