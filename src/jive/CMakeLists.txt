PROJECT(jive CXX)

MESSAGE( STATUS "<Jive>" )

FILE( GLOB decl y/jive/*.hpp)
FILE( GLOB impl y/jive/*.cpp)
SET( srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/y/jive )
SOURCE_GROUP( jive FILES ${srcs} )
 
########################################################################
## 
## lexical
##
########################################################################

FILE( GLOB lexical_decl y/jive/lexical/*.hpp)
FILE( GLOB lexical_impl y/jive/lexical/*.cpp)
SET( lexical_srcs ${lexical_decl} ${lexical_impl})
INSTALL( FILES ${lexical_decl} DESTINATION include/y/jive/lexical )
SOURCE_GROUP( jive/lexical FILES ${lexical_srcs} )

FILE( GLOB lexical_event_decl y/jive/lexical/event/*.hpp)
FILE( GLOB lexical_event_impl y/jive/lexical/event/*.cpp)
SET( lexical_event_srcs ${lexical_event_decl} ${lexical_event_impl})
INSTALL( FILES ${lexical_event_decl} DESTINATION include/y/jive/lexical/event )
SOURCE_GROUP( jive/lexical/event FILES ${lexical_event_srcs} )

FILE( GLOB lexical_plugin_decl y/jive/lexical/plugin/*.hpp)
FILE( GLOB lexical_plugin_impl y/jive/lexical/plugin/*.cpp)
SET( lexical_plugin_srcs ${lexical_plugin_decl} ${lexical_plugin_impl})
INSTALL( FILES ${lexical_plugin_decl} DESTINATION include/y/jive/lexical/plugin )
SOURCE_GROUP( jive/lexical/plugin FILES ${lexical_plugin_srcs} )


FILE( GLOB lexical_editor_decl y/jive/lexical/editor/*.hpp)
FILE( GLOB lexical_editor_impl y/jive/lexical/editor/*.cpp)
SET( lexical_editor_srcs ${lexical_editor_decl} ${lexical_editor_impl})
INSTALL( FILES ${lexical_editor_decl} DESTINATION include/y/jive/lexical/editor )
SOURCE_GROUP( jive/lexical/editor FILES ${lexical_editor_srcs} )

########################################################################
## 
## syntax
##
########################################################################
FILE( GLOB syntax_decl y/jive/syntax/*.hpp)
FILE( GLOB syntax_impl y/jive/syntax/*.cpp)
SET( syntax_srcs ${syntax_decl} ${syntax_impl})
INSTALL( FILES ${syntax_decl} DESTINATION include/y/jive/syntax )
SOURCE_GROUP( jive/syntax FILES ${syntax_srcs} )

FILE( GLOB syntax_grammar_decl y/jive/syntax/grammar/*.hpp)
FILE( GLOB syntax_grammar_impl y/jive/syntax/grammar/*.cpp)
SET( syntax_grammar_srcs ${syntax_grammar_decl} ${syntax_grammar_impl})
INSTALL( FILES ${syntax_grammar_decl} DESTINATION include/y/jive/syntax/grammar )
SOURCE_GROUP( jive/syntax/grammar FILES ${syntax_grammar_srcs} )

FILE( GLOB syntax_axiom_decl y/jive/syntax/axiom/*.hpp)
FILE( GLOB syntax_axiom_impl y/jive/syntax/axiom/*.cpp)
SET( syntax_axiom_srcs ${syntax_axiom_decl} ${syntax_axiom_impl})
INSTALL( FILES ${syntax_axiom_decl} DESTINATION include/y/jive/syntax/axiom )
SOURCE_GROUP( jive/syntax/axiom FILES ${syntax_axiom_srcs} )

FILE( GLOB syntax_axiom_wildcard_decl y/jive/syntax/axiom/wildcard/*.hpp)
FILE( GLOB syntax_axiom_wildcard_impl y/jive/syntax/axiom/wildcard/*.cpp)
SET( syntax_axiom_wildcard_srcs ${syntax_axiom_wildcard_decl} ${syntax_axiom_wildcard_impl})
INSTALL( FILES ${syntax_axiom_wildcard_decl} DESTINATION include/y/jive/syntax/axiom/wildcard )
SOURCE_GROUP( jive/syntax/axiom/wildcard FILES ${syntax_axiom_wildcard_srcs} )

FILE( GLOB syntax_axiom_compound_decl y/jive/syntax/axiom/compound/*.hpp)
FILE( GLOB syntax_axiom_compound_impl y/jive/syntax/axiom/compound/*.cpp)
SET( syntax_axiom_compound_srcs ${syntax_axiom_compound_decl} ${syntax_axiom_compound_impl})
INSTALL( FILES ${syntax_axiom_compound_decl} DESTINATION include/y/jive/syntax/axiom/compound )
SOURCE_GROUP( jive/syntax/axiom/compound FILES ${syntax_axiom_compound_srcs} )

FILE( GLOB syntax_xnode_decl y/jive/syntax/xnode/*.hpp)
FILE( GLOB syntax_xnode_impl y/jive/syntax/xnode/*.cpp)
SET( syntax_xnode_srcs ${syntax_xnode_decl} ${syntax_xnode_impl})
INSTALL( FILES ${syntax_xnode_decl} DESTINATION include/y/jive/syntax/xnode )
SOURCE_GROUP( jive/syntax/xnode FILES ${syntax_xnode_srcs} )

########################################################################
## 
## language
##
########################################################################
FILE( GLOB language_decl y/jive/language/*.hpp)
FILE( GLOB language_impl y/jive/language/*.cpp)
SET( language_srcs ${language_decl} ${language_impl})
INSTALL( FILES ${language_decl} DESTINATION include/y/jive/language )
SOURCE_GROUP( jive/language FILES ${language_srcs} )
 
FILE( GLOB language_axiom_decl y/jive/language/axiom/*.hpp)
FILE( GLOB language_axiom_impl y/jive/language/axiom/*.cpp)
SET( language_axiom_srcs ${language_axiom_decl} ${language_axiom_impl})
INSTALL( FILES ${language_axiom_decl} DESTINATION include/y/jive/language/axiom )
SOURCE_GROUP( jive/language/axiom FILES ${language_axiom_srcs} )

FILE( GLOB language_grammar_decl y/jive/language/grammar/*.hpp)
FILE( GLOB language_grammar_impl y/jive/language/grammar/*.cpp)
SET( language_grammar_srcs ${language_grammar_decl} ${language_grammar_impl})
INSTALL( FILES ${language_grammar_decl} DESTINATION include/y/jive/language/grammar )
SOURCE_GROUP( jive/language/grammar FILES ${language_grammar_srcs} )


FILE( GLOB language_xnode_decl y/jive/language/xnode/*.hpp)
FILE( GLOB language_xnode_impl y/jive/language/xnode/*.cpp)
SET( language_xnode_srcs ${language_xnode_decl} ${language_xnode_impl})
INSTALL( FILES ${language_xnode_decl} DESTINATION include/y/jive/language/xnode )
SOURCE_GROUP( jive/language/xnode FILES ${language_xnode_srcs} )


INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-jive STATIC
 ${srcs}
 ${lexical_srcs} ${lexical_event_srcs} ${lexical_plugin_srcs}
 ${lexical_editor_srcs}
 ${syntax_srcs} 
 ${syntax_grammar_srcs}
 ${syntax_axiom_srcs} ${syntax_axiom_wildcard_srcs} ${syntax_axiom_compound_srcs}
 ${syntax_xnode_srcs}
 ${language_srcs} 
 ${language_axiom_srcs} 
 ${language_xnode_srcs}
 ${language_grammar_srcs}
 )
INSTALL( TARGETS y-jive ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

SET(UTESTS "${UTESTS} utest-jive" PARENT_SCOPE)

MESSAGE( STATUS "<Jive/>" )
MESSAGE(STATUS "")

