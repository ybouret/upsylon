PROJECT(jargon)

FILE( GLOB decl y/jargon/*.hpp)
FILE( GLOB impl y/jargon/*.cpp)
SET( srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/y/jargon )
SOURCE_GROUP( jargon FILES ${srcs} )

FILE( GLOB axiom_decl y/jargon/axiom/*.hpp)
FILE( GLOB axiom_impl y/jargon/axiom/*.cpp)
SET( axiom_srcs ${axiom_decl} ${axiom_impl})
INSTALL( FILES ${axiom_decl} DESTINATION include/y/jargon/axiom )
SOURCE_GROUP( jargon/axiom FILES ${axiom_srcs} )

FILE( GLOB xnode_decl y/jargon/xnode/*.hpp)
FILE( GLOB xnode_impl y/jargon/xnode/*.cpp)
SET( xnode_srcs ${xnode_decl} ${xnode_impl})
INSTALL( FILES ${xnode_decl} DESTINATION include/y/jargon/xnode )
SOURCE_GROUP( jargon/xnode FILES ${xnode_srcs} )

FILE( GLOB lexical_decl y/jargon/lexical/*.hpp)
FILE( GLOB lexical_impl y/jargon/lexical/*.cpp)
SET( lexical_srcs ${lexical_decl} ${lexical_impl})
INSTALL( FILES ${lexical_decl} DESTINATION include/y/jargon/lexical )
SOURCE_GROUP( jargon/lexical FILES ${lexical_srcs} )

FILE( GLOB lexical_event_decl y/jargon/lexical/event/*.hpp)
FILE( GLOB lexical_event_impl y/jargon/lexical/event/*.cpp)
SET( lexical_event_srcs ${lexical_event_decl} ${lexical_event_impl})
INSTALL( FILES ${lexical_event_decl} DESTINATION include/y/jargon/lexical/event )
SOURCE_GROUP( jargon/lexical/event FILES ${lexical_event_srcs} )

FILE( GLOB lexical_plugin_decl y/jargon/lexical/plugin/*.hpp)
FILE( GLOB lexical_plugin_impl y/jargon/lexical/plugin/*.cpp)
SET( lexical_plugin_srcs ${lexical_plugin_decl} ${lexical_plugin_impl})
INSTALL( FILES ${lexical_plugin_decl} DESTINATION include/y/jargon/lexical/plugin )
SOURCE_GROUP( jargon/lexical/plugin FILES ${lexical_plugin_srcs} )


FILE( GLOB grammar_decl y/jargon/grammar/*.hpp)
FILE( GLOB grammar_impl y/jargon/grammar/*.cpp)
SET( grammar_srcs ${grammar_decl} ${grammar_impl})
INSTALL( FILES ${grammar_decl} DESTINATION include/y/jargon/grammar )
SOURCE_GROUP( jargon/grammar FILES ${grammar_srcs} )

FILE( GLOB axiom_compound_decl y/jargon/axiom/compound/*.hpp)
FILE( GLOB axiom_compound_impl y/jargon/axiom/compound/*.cpp)
SET( axiom_compound_srcs ${axiom_compound_decl} ${axiom_compound_impl})
INSTALL( FILES ${axiom_compound_decl} DESTINATION include/y/jargon/axiom/compound )
SOURCE_GROUP( jargon/axiom/compound FILES ${axiom_compound_srcs} )

FILE( GLOB axiom_wildcard_decl y/jargon/axiom/wildcard/*.hpp)
FILE( GLOB axiom_wildcard_impl y/jargon/axiom/wildcard/*.cpp)
SET( axiom_wildcard_srcs ${axiom_wildcard_decl} ${axiom_wildcard_impl})
INSTALL( FILES ${axiom_wildcard_decl} DESTINATION include/y/jargon/axiom/wildcard )
SOURCE_GROUP( jargon/axiom/wildcard FILES ${axiom_wildcard_srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-jargon STATIC
 ${srcs}
 ${xnode_srcs}
 ${axiom_srcs}
 ${axiom_compound_srcs}
 ${axiom_wildcard_srcs}
 ${grammar_srcs}
 ${lexical_srcs} ${lexical_event_srcs} ${lexical_plugin_srcs} 
 )
INSTALL( TARGETS y-jargon ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
