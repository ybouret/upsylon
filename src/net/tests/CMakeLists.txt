PROJECT(test-net)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-net main.cpp ${srcs})
TARGET_LINK_Y(test-net net)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(Network test-net network)
ADD_TEST(BSD     test-net bsd)
ADD_TEST(Addr    test-net addr)
ADD_TEST(Resolve test-net resolve localhost@8080)
ADD_TEST(Info    test-net info)
ADD_TEST(SockSet test-net sockset)
ADD_TEST(Comm:Bytes  test-net comm_bytes)
ADD_TEST(Comm:Queues test-net comm_queues)
ADD_TEST(Comm:Proto  test-net comm_proto)

ADD_CUSTOM_TARGET(utest-net
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-net)

INSTALL( TARGETS test-net RUNTIME DESTINATION var/y )
