PROJECT(jive CXX)

MESSAGE( STATUS "<Jive>" )


FILE( GLOB decl y/jive/*.hpp)
FILE( GLOB impl y/jive/*.cpp)
SET( srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/y/jive )
SOURCE_GROUP( jive FILES ${srcs} )
 
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




INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-jive STATIC
 ${srcs}
 ${lexical_srcs} ${lexical_event_srcs} ${lexical_plugin_srcs}
 )
INSTALL( TARGETS y-jive ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

SET(UTESTS "${UTESTS} utest-jive" PARENT_SCOPE)

MESSAGE( STATUS "<Jive/>" )
MESSAGE(STATUS "")

