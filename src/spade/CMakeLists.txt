PROJECT(spade)


FILE( GLOB decl y/spade/*.hpp)
FILE( GLOB impl y/spade/*.cpp)
SET( srcs ${decl} ${impl} )

INSTALL( FILES ${decl} DESTINATION include/y/spade )
SOURCE_GROUP(spade FILES ${srcs} )

FILE( GLOB field_decl y/spade/field/*.hpp)
FILE( GLOB field_impl y/spade/field/*.cpp)
SET( field_srcs ${field_decl} ${field_impl} )

INSTALL( FILES ${field_decl} DESTINATION include/y/spade/field )
SOURCE_GROUP(spade/field FILES ${field_srcs} )

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY( y-spade STATIC ${srcs} ${field_srcs}  )
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
