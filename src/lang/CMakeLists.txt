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

FILE(GLOB syntax_node_decl y/lang/syntax/node/*.hpp)
FILE(GLOB syntax_node_code y/lang/syntax/node/*.cpp)
SET(syntax_node_srcs ${syntax_node_decl} ${syntax_node_code})
SOURCE_GROUP( lang/syntax/node FILES ${syntax_node_srcs} )
INSTALL( FILES ${syntax_node_decl} DESTINATION include/y/lang/syntax/node )


FILE(GLOB syntax_rule_decl y/lang/syntax/rule/*.hpp)
FILE(GLOB syntax_rule_code y/lang/syntax/rule/*.cpp)
SET(syntax_rule_srcs ${syntax_rule_decl} ${syntax_rule_code})
SOURCE_GROUP( lang/syntax/rule FILES ${syntax_rule_srcs} )
INSTALL( FILES ${syntax_rule_decl} DESTINATION include/y/lang/syntax/rule )

FILE(GLOB dynamo_decl y/lang/dynamo/*.hpp)
FILE(GLOB dynamo_code y/lang/dynamo/*.cpp)
SET(dynamo_srcs ${dynamo_decl} ${dynamo_code})
SOURCE_GROUP( lang/dynamo FILES ${dynamo_srcs} )
INSTALL( FILES ${dynamo_decl} DESTINATION include/y/lang/dynamo )

FILE(GLOB dynamo_core_decl y/lang/dynamo/core/*.hpp)
FILE(GLOB dynamo_core_code y/lang/dynamo/core/*.cpp)
SET(dynamo_core_srcs ${dynamo_core_decl} ${dynamo_core_code})
SOURCE_GROUP( lang/dynamo/core FILES ${dynamo_core_srcs} )
INSTALL( FILES ${dynamo_core_decl} DESTINATION include/y/lang/dynamo/core )

FILE(GLOB dynamo_gen_decl y/lang/dynamo/gen/*.hpp)
FILE(GLOB dynamo_gen_code y/lang/dynamo/gen/*.cpp)
SET(dynamo_gen_srcs ${dynamo_gen_decl} ${dynamo_gen_code})
SOURCE_GROUP( lang/dynamo/gen FILES ${dynamo_gen_srcs} )
INSTALL( FILES ${dynamo_gen_decl} DESTINATION include/y/lang/dynamo/gen )

ADD_LIBRARY(y-lang STATIC 
${main_srcs} 
${syntax_srcs}
${syntax_grammar_srcs}
${syntax_rule_srcs}
${syntax_node_srcs}
${dynamo_srcs} ${dynamo_core_srcs} ${dynamo_gen_srcs}
)
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-lang" PARENT_SCOPE)

MESSAGE( STATUS "<Lang/>")
MESSAGE(STATUS "")
