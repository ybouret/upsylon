MESSAGE( STATUS "")
MESSAGE( STATUS "External Projects: preparing..." )

MESSAGE( STATUS "          zlib")
ADD_SUBDIRECTORY(y/zlib)

MESSAGE( STATUS "External Projects are prepared!")
MESSAGE( STATUS "")

ADD_CUSTOM_TARGET(ext 
DEPENDS 
y-z    test-z
COMMENT "Tested External Projects")


