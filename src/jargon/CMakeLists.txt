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

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-jargon STATIC
 ${srcs}
 ${axiom_srcs}
   )
INSTALL( TARGETS y-jargon ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
