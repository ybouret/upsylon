
FOREACH( word IN ITEMS "-Wextra" )
	STRING(REPLACE "${word}" "" CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}"   )
	STRING(REPLACE "${word}" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )
ENDFOREACH()

MESSAGE( STATUS "                   lua")
ADD_SUBDIRECTORY(y/lua)

MESSAGE( STATUS "                   zlib")
ADD_SUBDIRECTORY(y/zlib)

MESSAGE( STATUS "                   jbig")
ADD_SUBDIRECTORY(y/jbig)

MESSAGE( STATUS "                   bz2")
ADD_SUBDIRECTORY(y/bz2)

MESSAGE( STATUS "                   png")
ADD_SUBDIRECTORY(y/png)

MESSAGE( STATUS "                   jpeg")
ADD_SUBDIRECTORY(y/jpeg)

MESSAGE( STATUS "                   tiff")
ADD_SUBDIRECTORY(y/tiff)

MESSAGE( STATUS "                   sqlite")
ADD_SUBDIRECTORY(y/sqlite)

ADD_CUSTOM_TARGET(ext 
DEPENDS 
y-z       utest-z
y-jbig    utest-jbig
y-bz2     utest-bz2
y-png     utest-png
y-jpeg    utest-jpeg
y-tiff 
y-lua     utest-lua
y-sqlite3 y_sqlite
COMMENT "External Projects are now tested!!!")


