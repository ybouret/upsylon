PROJECT(y-net CXX)

MESSAGE( STATUS "<NETwork>" )

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/net/*.hpp)
FILE(GLOB main_code y/net/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( net FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/net )

FILE(GLOB socket_decl y/net/socket/*.hpp)
FILE(GLOB socket_code y/net/socket/*.cpp)
SET(socket_srcs ${socket_decl} ${socket_code})
SOURCE_GROUP( net/socket FILES ${socket_srcs} )
INSTALL( FILES ${socket_decl} DESTINATION include/y/net/socket )

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

FILE(GLOB comm_decl y/net/comm/*.hpp)
FILE(GLOB comm_code y/net/comm/*.cpp)
SET(comm_srcs ${comm_decl} ${comm_code})
SOURCE_GROUP( net/comm FILES ${comm_srcs} )
INSTALL( FILES ${comm_decl} DESTINATION include/y/net/comm )

IF(FALSE)
FILE(GLOB io_decl y/net/io/*.hpp)
FILE(GLOB io_code y/net/io/*.cpp)
SET(io_srcs ${io_decl} ${io_code})
SOURCE_GROUP( net/io FILES ${io_srcs} )
INSTALL( FILES ${io_decl} DESTINATION include/y/net/io )
ENDIF()

ADD_LIBRARY(y-net STATIC 
${main_srcs}
${socket_srcs}
${tcp_srcs}
${udp_srcs}
${comm_srcs}
)
INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-net" PARENT_SCOPE)

MESSAGE( STATUS "<NETwork/>" )
MESSAGE( STATUS "" )
