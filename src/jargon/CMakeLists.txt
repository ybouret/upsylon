PROJECT(jargon)

FILE( GLOB decl y/jargon/*.hpp)
FILE( GLOB impl y/jargon/*.cpp)
SET( srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/y/jargon )
SOURCE_GROUP( jargon FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-jargon STATIC ${srcs}  )
INSTALL( TARGETS y-jargon ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
