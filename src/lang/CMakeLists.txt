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

FILE(GLOB lexical_decl y/lang/lexical/*.hpp)
FILE(GLOB lexical_code y/lang/lexical/*.cpp)
SET(lexical_srcs ${lexical_decl} ${lexical_code})
SOURCE_GROUP( lang/lexical FILES ${lexical_srcs} )
INSTALL( FILES ${lexical_decl} DESTINATION include/y/lang/lexical )

FILE(GLOB lexical_plugin_decl y/lang/lexical/plugin/*.hpp)
FILE(GLOB lexical_plugin_code y/lang/lexical/plugin/*.cpp)
SET(lexical_plugin_srcs ${lexical_plugin_decl} ${lexical_plugin_code})
SOURCE_GROUP( lang/lexical/plugin FILES ${lexical_plugin_srcs} )
INSTALL( FILES ${lexical_plugin_decl} DESTINATION include/y/lang/lexical/plugin )

FILE(GLOB syntax_decl y/lang/syntax/*.hpp)
FILE(GLOB syntax_code y/lang/syntax/*.cpp)
SET(syntax_srcs ${syntax_decl} ${syntax_code})
SOURCE_GROUP( lang/syntax FILES ${syntax_srcs} )
INSTALL( FILES ${syntax_decl} DESTINATION include/y/lang/syntax )

FILE(GLOB dynamo_decl y/lang/dynamo/*.hpp)
FILE(GLOB dynamo_code y/lang/dynamo/*.cpp)
SET(dynamo_srcs ${dynamo_decl} ${dynamo_code})
SOURCE_GROUP( lang/dynamo FILES ${dynamo_srcs} )
INSTALL( FILES ${dynamo_decl} DESTINATION include/y/lang/dynamo )

ADD_LIBRARY(y-lang STATIC 
${main_srcs} 
${pattern_srcs}
${lexical_srcs}
${lexical_plugin_srcs}
${syntax_srcs}
${dynamo_srcs}
)
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(y-dynamo y/lang/dynamo/y-dynamo.cxx)
TARGET_LINK_Y(y-dynamo lang)


ADD_SUBDIRECTORY(tests)
