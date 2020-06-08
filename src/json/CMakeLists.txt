PROJECT(json)
MESSAGE( STATUS "<JSON>")


FILE( GLOB decl y/json/*.hpp)
FILE( GLOB impl y/json/*.cpp)
SET( srcs ${decl} ${impl} 
)
INSTALL( FILES ${decl} DESTINATION include/y/json )
SOURCE_GROUP( json FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../jargon)

ADD_LIBRARY( y-json STATIC ${srcs}  )
INSTALL( TARGETS y-json ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-json" PARENT_SCOPE)

MESSAGE( STATUS "<JSON/>")
MESSAGE(STATUS "")
