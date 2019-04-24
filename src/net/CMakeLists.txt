PROJECT(y-net CXX)

MESSAGE( STATUS "<NETwork>" )

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/net/*.hpp)
FILE(GLOB main_code y/net/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( net FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/net )


FILE(GLOB tcp_decl y/net/tcp/*.hpp)
FILE(GLOB tcp_code y/net/tcp/*.cpp)
SET(tcp_srcs ${tcp_decl} ${tcp_code})
SOURCE_GROUP( net/tcp FILES ${tcp_srcs} )
INSTALL( FILES ${tcp_decl} DESTINATION include/y/net/tcp )

FILE(GLOB udp_decl y/net/udp/*.hpp)
FILE(GLOB udp_code y/net/udp/*.cpp)
SET(udp_srcs ${udp_decl} ${udp_code})
SOURCE_GROUP( net/udp FILES ${udp_srcs} )
INSTALL( FILES ${udp_decl} DESTINATION include/y/net/udp )


ADD_LIBRARY(y-net STATIC 
${main_srcs}
${tcp_srcs}
${udp_srcs}
)
INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
MESSAGE( STATUS "<NETwork/>" )
MESSAGE( STATUS "" )
