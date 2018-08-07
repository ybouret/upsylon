MESSAGE( STATUS "")
MESSAGE( STATUS "External Projects: preparing..." )

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

MESSAGE( STATUS "External Projects are prepared!")
MESSAGE( STATUS "")

ADD_CUSTOM_TARGET(ext 
DEPENDS 
y-z    utest-z
y-jbig utest-jbig
y-bz2  utest-bz2
y-png  utest-png
y-jpeg utest-jpeg
y-tiff 
y-lua
COMMENT "Tested External Projects")


