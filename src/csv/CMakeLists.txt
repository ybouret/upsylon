PROJECT(csv)
MESSAGE( STATUS "<CSV>")

FILE( GLOB decl y/csv/*.hpp)
FILE( GLOB impl y/csv/*.cpp)
SET( srcs ${decl} ${impl} 
)
INSTALL( FILES ${decl} DESTINATION include/y/csv )
SOURCE_GROUP( csv FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../jive)

ADD_LIBRARY( y-csv STATIC ${srcs}  )
INSTALL( TARGETS y-csv ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-csv" PARENT_SCOPE)

MESSAGE( STATUS "<CSV/>")
MESSAGE(STATUS "")
