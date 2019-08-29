
FOREACH( word IN ITEMS "-Wextra" )
	STRING(REPLACE "${word}" "" CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}"   )
	STRING(REPLACE "${word}" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )
ENDFOREACH()

SET(EXT_TESTS "" )

MESSAGE( STATUS "                   lua")
ADD_SUBDIRECTORY(y/lua)
SET(EXT_TESTS "${EXT_TESTS} utest-lua")

MESSAGE( STATUS "                   zlib")
ADD_SUBDIRECTORY(y/zlib)
SET(EXT_TESTS "${EXT_TESTS} utest-z")

MESSAGE( STATUS "                   jbig")
ADD_SUBDIRECTORY(y/jbig)
SET(EXT_TESTS "${EXT_TESTS} utest-jbig")

MESSAGE( STATUS "                   bz2")
ADD_SUBDIRECTORY(y/bz2)
SET(EXT_TESTS "${EXT_TESTS} utest-bz2")

MESSAGE( STATUS "                   png")
ADD_SUBDIRECTORY(y/png)
SET(EXT_TESTS "${EXT_TESTS} utest-png")

MESSAGE( STATUS "                   jpeg")
ADD_SUBDIRECTORY(y/jpeg)
SET(EXT_TESTS "${EXT_TESTS} utest-jpeg")

MESSAGE( STATUS "                   tiff")
ADD_SUBDIRECTORY(y/tiff)
SET(EXT_TESTS "${EXT_TESTS} y-tiff")

MESSAGE( STATUS "                   sqlite")
ADD_SUBDIRECTORY(y/sqlite)
SET(EXT_TESTS "${EXT_TESTS} y_sqlite")

SET(UTESTS "${UTESTS} ${EXT_TESTS}" PARENT_SCOPE)



