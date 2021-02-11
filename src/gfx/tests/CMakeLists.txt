PROJECT(test-gfx)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-gfx gfx)

 
ENABLE_TESTING()
ADD_TEST(types  test-gfx types)
ADD_TEST(area    test-gfx area)
ADD_TEST(tile    test-gfx tile)
ADD_TEST(tiles   test-gfx tiles)
ADD_TEST(engine  test-gfx engine)
ADD_TEST(pixels  test-gfx pixels) 
ADD_TEST(bitmap  test-gfx bitmap)
ADD_TEST(pixmap  test-gfx pixmap)
ADD_TEST(broker  test-gfx broker)
ADD_TEST(grad    test-gfx grad)

ADD_CUSTOM_TARGET(utest-gfx
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-gfx)

INSTALL( TARGETS test-gfx RUNTIME DESTINATION var/y )
