PROJECT(spade)


FILE( GLOB decl y/spade/*.hpp)
FILE( GLOB impl y/spade/*.cpp)
SET( srcs ${decl} ${impl} 
)
INSTALL( FILES ${decl} DESTINATION include/y/spade )
SOURCE_GROUP(spade FILES ${srcs} )

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY( y-spade STATIC ${srcs}  )
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
